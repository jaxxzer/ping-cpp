// Adopted from original example here:
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#overview

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "../src/message/ping-message-ping360.h"
#include "../src/message/ping-parser.h"


int main()
{
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyUSB0", O_RDWR);

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }



    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 42;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    // cfsetispeed(&tty, B115200);
    // cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B2000000);
    cfsetospeed(&tty, B2000000);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    int breakCode = tcsendbreak(serial_port, 0);

    if (breakCode != 0) {
        printf("Error sending break");
    }

ping360_transducer message;
message.set_mode(1);
message.set_gain_setting(0);
message.set_angle(0);
message.set_transmit_duration(50);
message.set_sample_period(80);
message.set_transmit_frequency(740);
message.set_number_of_samples(200);
message.set_transmit(1);
message.set_reserved(0);

int angle = 0;

// Allocate memory for read buffer, set size according to your needs
char read_buf [256];
memset(&read_buf, '\0', sizeof(read_buf));

write(serial_port, "U", 1);

PingParser parser(512);

for (int i = 0; i < 400; i++) {
    message.set_angle(i);
    message.updateChecksum();
    write(serial_port, (char*)message.msgData, message.msgDataLength());
    //write(serial_port, msg, sizeof(msg));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = 0;
    parser.reset();
    PingParser::ParseState state;
    while (num_bytes < 224) {
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        int n = read(serial_port, &read_buf, sizeof(read_buf));
        if (num_bytes < 0) {
            printf("Error reading: %s\n", strerror(errno));
            num_bytes = 224;
        } else {
            num_bytes += n;
            for (int i = 0; i < n; i++) {
                state = parser.parseByte(read_buf[i]);
                switch (state) {
                    case PingParser::NEW_MESSAGE:
                    if ((Ping360Id)parser.rxMessage.message_id() == Ping360Id::DEVICE_DATA) {
                        ping360_device_data response(parser.rxMessage);
                        printf("\ngot new message:\n");
                        for (int j = 0; j < response.msgDataLength(); j++) {
                            printf("%d,", response.msgData[j]);
                        }
                    }
                    break;
                    case PingParser::ERROR:
                    printf("\n\n\ngot error\n\n\n");
                    break;
                    default:
                    break;
                }
            }
        }
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    //printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
   
}

    close(serial_port);
}
