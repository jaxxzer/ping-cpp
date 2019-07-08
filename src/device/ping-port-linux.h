#pragma once

#include "ping-port.h"
#include <cstdio>

class PingPortLinux : public PingPort
{
public:
    PingPortLinux(const char* filename);
    ~PingPortLinux() {
        if (_handle) {
            std::fclose(_handle);
        }
    }

    virtual int read(char* buffer, int nBytes) override final;
    virtual int write(const char* data, int nBytes) override final;
private:
    void sendBreak(int milliseconds);
    bool setBaudrate(int baudrate);
    std::FILE* _handle;
};