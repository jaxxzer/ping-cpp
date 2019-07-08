#include <stdio.h>
#include <termios.h> // Contains POSIX terminal control definitions

#include "../src/device/ping-port-linux.h"
#include "../src/device/ping-device.h"
#include "../src/message/ping-message-common.h"

static const char* portName = "/dev/ttyUSB0";
PingPortLinux port = PingPortLinux(portName);
PingDevice device = PingDevice(port);

int main()
{
    //port.setBaudrate(B115200);
    // sendBreak(0);
    // write("U", 1);

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
    control.set_mode(1);
    control.set_number_of_samples(200);
    control.set_reserved(0);
    control.set_sample_period(80);
    control.set_transmit_frequency(740);
    control.set_transmit_duration(100);
    control.set_transmit(1);

    for (int i = 0; i < 400; i++) {
        control.set_angle(i);
        control.updateChecksum();
        printf("control transducer\n");
        device.write((char*)control.msgData, control.msgDataLength());
        ping360_device_data* reply = (ping360_device_data*)device.waitMessage(Ping360Id::DEVICE_DATA, 4000);
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

    return 0;
}
