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

#include "../src/device/ping-port-linux.h"
#include "../src/device/ping-device.h"
#include "../src/message/ping-message-common.h"
#include "../src/message/ping-message-ping1d.h"

static const char* portName = "/dev/ttyUSB0";
PingPortLinux port = PingPortLinux(portName);
PingDevice device = PingDevice(port);

int main()
{
    common_protocol_version* msg = (common_protocol_version*)device.request(CommonId::PROTOCOL_VERSION);

    if (!msg) {
        printf("Error getting protocol_version message\n");
        return 1;
    }

    printf("protocol version %d.%d.%d\n",
        msg->version_major(),
        msg->version_minor(),
        msg->version_major());
}
