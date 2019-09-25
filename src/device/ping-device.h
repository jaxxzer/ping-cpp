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
   *  @param timeout_ms: The timeout period to wait for the requested
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
  ping_message* request(uint16_t id, int timeout_ms = 500);

  /**
   *  @brief Wait for receipt of a message with a particular message id from
   * device
   *
   *  @param id: The message id to wait for
   *  @param timeout_ms: The timeout period to wait for a matching ping_message
   * to be received
   *
   *  @return The ping_message received with matching id
   *  @return null if the timeout expires and no ping_message was received with
   * a matching id
   */
  ping_message* waitMessage(uint16_t id, int timeout_ms = 500);

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
   *  @brief Write a ping_message to the device.
   *
   *  @param msg: a ping_message object. Note that the caller must ensure that
   * the \n ping_message object is prepared (call msg.updateChecksum first)
   */
  void writeMessage(const ping_message& msg);

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
   *  @param msg: A pointer to the message received from the device
   */
  virtual void _handleMessage(ping_message* msg);

private:
  PingParser _parser;
  PingPort& _port;
};
