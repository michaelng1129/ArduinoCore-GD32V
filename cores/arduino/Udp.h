// Udp.h - UDP interface, Arduino API compatible.
// Mirrors ArduinoCore-API's api/Udp.h.

#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>
#include "Stream.h"
#include "IPAddress.h"

class UDP : public Stream {
public:
    virtual uint8_t begin(uint16_t port) = 0;
    virtual void stop() = 0;
    virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
    virtual int beginPacket(const char *host, uint16_t port) = 0;
    virtual int endPacket() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual int parsePacket() = 0;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int read(unsigned char *buffer, size_t len) = 0;
    virtual int read(char *buffer, size_t len) = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual IPAddress remoteIP() = 0;
    virtual uint16_t remotePort() = 0;
};

#endif
