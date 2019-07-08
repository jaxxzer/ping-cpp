#pragma once

#include "ping-port.h"

#include "../message/ping-message-ping360.h"
#include "../message/ping-parser.h"

class PingDevice
{
public:

    /**
     *  @brief Constructor
     *
     *  @param port: The device I/O
     */
    PingDevice(PingPort& port)  : _port(port)
                                , _parser(4096)
    {}
 
    /**
     *  @brief Read in data from device, return a PingMessage if available.
     *  Data will be read in from device until there is no data left in the RX buffer,
     *  or a valid PingMessage is successfully decoded.
     *  Note that there may still be data available in the RX buffer for decoding when
     *  this function returns a PingMessage.
     *
     *  @return: The next PingMessage from the device
     *  @return: null if the RX buffer is empty and no PingMessage has been decoded
     */
    ping_message* read();

    /**
     *  @brief Write data to device
     *
     *  @param data: pointer to buffer to write
     *  @param length: buffer length to write
     *
     *  @return: size of data buffer written to the device
     */
    int write(const char* data, int length);

    /**
     *  @brief Wait for receipt of a message with a particular message id from device
     *
     *  @param id: The message id to wait for
     *  @param timeout_ms: The timeout period to wait for a matching ping_message to be received
     *
     *  @return The ping_message received with matching id
     *  @return null if the timeout expires and no ping_message was received with a matching id
     */
    ping_message* waitMessage(uint16_t id, int timeout_ms = 500);

    /**
     *  @brief Request a ping_message from the device
     *
     *  @param id: The message ID to request
     *  @param timeout_ms: The timeout period to wait for the requested ping_message to be received
     *
     *  @return The ping_message that was requested
     *  @return null if the device did not reply with the requested message before the timeout period expired
     *
     *  @par ex.
     *  @code
     *  ping_msg_ping1D_voltage_5 msg(*pd.request(Ping1dId::Voltage_5));
     *  @endcode
     */
    ping_message* request(uint16_t id, int timeout_ms = 500);

    /**
     *  @brief Handle an incoming message from the device. Internal values are updated according to the device data.
     *
     *  @param pmsg: The message received from the device
     */
    void handleMessage(ping_message* pmsg);
    
    uint8_t device_type;
    uint8_t device_revision;
    uint8_t firmware_version_major;
    uint8_t firmware_version_minor;
    uint8_t firmware_version_patch;
    uint8_t version_major;
    uint8_t version_minor;
    uint8_t version_patch;

private:
    PingPort& _port;
    PingParser _parser;
};
