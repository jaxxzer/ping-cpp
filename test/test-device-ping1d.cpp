#include <stdio.h>

#include "ping-port-linux.h"
#include "ping-device-ping1d.h"
#include "ping-message-common.h"
#include "ping-message-ping1d.h"

static const char* portName = "/dev/ttyUSB0";
PingPortLinux port = PingPortLinux(portName);
Ping1d device = Ping1d(port);

int main()
{
    printf("initializing\n");

    if (!device.initialize(100)) {
        printf("device initialization failed\n");
    }

    if (!device.set_mode_auto(false)) {
        printf("failed to set device mode auto\n");
    }
    
    if (!device.set_range(100, 30000)) {
        printf("failed to set scan range\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("requesting profile\n");
        if (!device.request(Ping1dId::PROFILE, 4000)) {
            printf("no reply\n");
        } else {
            printf("profile data: ");
            for (int n = 0; n < device.profile_data_length(); n++) {
                printf("%d,", device.profile_data()[n]);
            }
            printf("\n");
        }
    }

    return 0;
}
