#pragma once

#include "ping-device.h"

class Ping360 : public PingDevice
{
public:
    void control_reset();
    void control_transducer();
    ping360_device_data* get_device_data();
    bool set_device_id(uint8_t id);

    ping360_device_data* set_mode();
    ping360_device_data* set_gain_setting();
    ping360_device_data* set_angle();
    ping360_device_data* set_transmit_duration();
    ping360_device_data* set_sample_period();
    ping360_device_data* set_transmit_frequency();
    ping360_device_data* set_number_of_samples();   

    ping360_device_data* readParameters();
    ping360_device_data* transmitAngle(uint16_t angle);
    ping360_device_data* transmit(); 

};
