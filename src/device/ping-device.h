#pragma once

#include <ping-parser.h>
#include <ping-port.h>

/**
 * A PingDevice class can be used to communicate with a
 * device implementing Blue Robotics ping-protocol*
 *
 * *only messages from the common message set are handled by the PingDevice
 * base class
 *
 * to use another message set, you should subclass PingDevice
 * and re-implement handleMessage
 */
class PingDevice {
public:
  /**
   *  @brief Constructor
   *
   *  @param port: The device io handle
   */
  PingDevice(PingPort& port) : _port(port), _parser(4096) {}

  /**
   *  @brief Initialize the device by establishing communications and reading in some basic data
   * 
   *  @return true if the device was successfully initialized
   */
  virtual bool initialize();

  /**
   *  @brief Read in data from device, return a PingMessage if available.
   *  Data will be read in from device until there is no data left in the RX
   * buffer, or a valid PingMessage is successfully decoded. Note that there may
   * still be data available in the RX buffer for decoding when this function
   * returns a PingMessage.
   *
   *  @return: The next PingMessage from the device
   *  @return: null if the RX buffer is empty and no PingMessage has been
   * decoded
   */
  ping_message* read();

  /**
   *  @brief Request a ping_message from the device
   *
   *  @param id: The message ID to request
   *  @param timeoutMs: The timeout period to wait for the requested
   * ping_message to be received
   *
   *  @return The ping_message that was requested
   *  @return null if the device did not reply with the requested message before
   * the timeout period expired
   *
   *  @par ex.
   *  @code
   *  ping_msg_ping1D_voltage_5 msg(*pd.request(Ping1dId::Voltage_5));
   *  @endcode
   */
  ping_message* request(uint16_t id, int timeoutMs = 500);

  /**
   *  @brief Wait for receipt of a message with a particular message id from
   * device
   *
   *  @param id: The message id to wait for
   *  @param timeoutMs: The timeout period to wait for a matching ping_message
   * to be received
   *
   *  @return The ping_message received with matching id
   *  @return null if the timeout expires and no ping_message was received with
   * a matching id
   */
  ping_message* waitMessage(uint16_t id, int timeoutMs = 500);

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
   *  @brief Write a ping_message to the device. Before writing, the ping_messaage is \n
   *  prepared by updating the checksum
   *
   *  @param message: a pointer to the ping_message
   */
  void writeMessage(ping_message& message);

  uint8_t device_type;
  uint8_t device_revision;
  uint8_t firmware_version_major;
  uint8_t firmware_version_minor;
  uint8_t firmware_version_patch;
  uint8_t version_major;
  uint8_t version_minor;
  uint8_t version_patch;

protected:
  /**
   *  @brief Handle an incoming message from the device. This object's values \n
   *  are updated according to the device data.
   *
   *  @param message: A pointer to the message received from the device
   */
  virtual void _handleMessage(ping_message* message);

private:
  PingParser _parser;
  PingPort& _port;
};
