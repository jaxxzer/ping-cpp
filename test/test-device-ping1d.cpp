#include <stdio.h>

#include "ping-port-linux.h"
#include "ping-device-ping1d.h"
#include "ping-message-common.h"
#include "ping-message-ping1d.h"

static const char* portName = "/dev/ttyUSB0";


int main()
{
    PingPortLinux port = PingPortLinux(portName);
    Ping1d device = Ping1d(port);

    printf("initializing\n");

    if (!device.initialize(100)) {
        printf("device initialization failed\n");
    } else {
        printf("pass\n");
    }

    printf("set mode auto\n");

    if (!device.set_mode_auto(false)) {
        printf("failed to set device mode auto\n");
    } else {
        printf("pass");
    }
    
    printf("set range");
    if (!device.set_range(100, 30000)) {
        printf("failed to set scan range\n");
    } else {
        printf("pass\n");
    }

    printf("requesting profile\n");
    if (!device.request(Ping1dId::PROFILE, 4000)) {
        printf("no reply\n");
    } else {
        printf("pass");
    }

    return 0;
}
