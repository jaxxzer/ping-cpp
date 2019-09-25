#include <ping-device.h>

#include <ping-message.h>
#include <ping-message-common.h>
#include <ping-time.h>

ping_message* PingDevice::read()
{
    char b;
    int result = _port.read(&b, 1);
    if (result > 0) {
        if(_parser.parseByte((char)b) == PingParser::NEW_MESSAGE) {
            return &_parser.rxMessage;
        }
    }
    return nullptr;
}

int PingDevice::write(const char* data, int nBytes)
{
    return _port.write(data, nBytes);
}

void PingDevice::writeMessage(ping_message& msg)
{
    msg.updateChecksum();
    write((char*)msg.msgData, msg.msgDataLength());
}

ping_message* PingDevice::waitMessage(uint16_t id, int timeout_ms)
{
    int tstart = PingTime::time_ms();
    while (PingTime::time_ms() < tstart + timeout_ms) {
        ping_message* pmsg = read();
        
        if (!pmsg) {
            continue;
        }

        handleMessage(pmsg);

        if (pmsg->message_id() == CommonId::NACK) {
            return nullptr;
        }

        if (pmsg->message_id() == id) {
            return pmsg;
        }
    }
    return nullptr;
}

ping_message* PingDevice::request(uint16_t id, int timeout_ms)
{
    common_general_request msg;
    msg.set_requested_id(id);
    writeMessage(msg);
    return waitMessage(id, timeout_ms);
}

void PingDevice::handleMessage(ping_message* pmsg)
{
    switch (pmsg->message_id()) {
    case CommonId::NACK:
    {
        common_nack msg(*pmsg);
        break;
    }
    case CommonId::PROTOCOL_VERSION:
    {
        common_protocol_version msg(*pmsg);

        version_major = msg.version_major();
        version_minor = msg.version_minor();
        version_patch = msg.version_patch();
        break;
    }
    case CommonId::DEVICE_INFORMATION:
    {
        common_device_information msg(*pmsg);
        device_type = msg.device_type();
        device_revision = msg.device_revision();
        firmware_version_major = msg.firmware_version_major();
        firmware_version_minor = msg.firmware_version_minor();
        firmware_version_patch = msg.firmware_version_patch();
        break;
    }
    default:
        break;
    }
}
