#include <stdio.h>
#include <termios.h> // Contains POSIX terminal control definitions

#include "ping-time.h"
#include "../src/device/ping-port-linux.h"
#include "../src/device/ping-device.h"
#include "../src/message/ping-message-common.h"

static const char* portName = "/dev/ttyUSB0";
PingPortLinux port = PingPortLinux(portName);
PingDevice device = PingDevice(port);

const int samples = 200;
const int duration = 5;
const int period = 80;

int main()
{
    port.setBaudrate(B2000000);
    port.sendBreak(0);
    PingTime::microsecondDelay(11000);
    port.write("U", 1);
    PingTime::microsecondDelay(11000);

    common_protocol_version* msg = (common_protocol_version*)device.request(CommonId::PROTOCOL_VERSION);

    if (!msg) {
        printf("Error getting protocol_version message\n");
        return 1;
    }

    printf("protocol version %d.%d.%d\n",
        msg->version_major(),
        msg->version_minor(),
        msg->version_major());

    ping360_transducer control;
    control.set_gain_setting(0);
    control.set_angle(0);
    control.set_mode(1);
    control.set_number_of_samples(samples);
    control.set_reserved(0);
    control.set_sample_period(period);
    control.set_transmit_frequency(740);
    control.set_transmit_duration(duration);
    control.set_transmit(1);
    control.updateChecksum();
    
    // perform homing operation before we start stopwatch timer
    printf("control transducer\n");
    device.write((char*)control.msgData, control.msgDataLength());
    ping360_device_data* reply = (ping360_device_data*)device.waitMessage(Ping360Id::DEVICE_DATA, 4500);
    if (!reply) {
        printf("no reply\n");
    }

    int tstart_ms = PingTime::time_ms();

    for (int i = 0; i < 400; i++) {
        control.set_angle(i);
        control.updateChecksum();
        printf("control transducer\n");
        device.write((char*)control.msgData, control.msgDataLength());
        ping360_device_data* reply = (ping360_device_data*)device.waitMessage(Ping360Id::DEVICE_DATA, 4500);
        if (!reply) {
            printf("no reply\n");
        } else {
            printf("reply:");
            for (int i = 0; i < reply->msgDataLength(); i++) {
                printf("%d,", reply->msgData[i]);
            }
            printf("\n");
        }
    }

    int tend_ms = PingTime::time_ms();

    printf("\nfull scan in %d ms\n", tend_ms - tstart_ms);
    printf("pulse duration: %d\n", duration);
    printf("sample period: %d\n", period);
    printf("number of samples: %d\n", samples);
    return 0;
}
