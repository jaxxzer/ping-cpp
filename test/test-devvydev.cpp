#include <stdio.h>

#include "../src/device/ping-port-linux.h"
#include "../src/device/ping-device.h"
#include "../src/message/ping-message-common.h"

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

    return 0;
}
