// Client.h - TCP client interface, Arduino API compatible.
// Mirrors ArduinoCore-API's api/Client.h.

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Print.h"
#include "Stream.h"
#include "IPAddress.h"

class Client : public Stream {
public:
    virtual int connect(IPAddress ip, uint16_t port) = 0;
    virtual int connect(const char *host, uint16_t port) = 0;
    virtual uint8_t connected() = 0;
    virtual operator bool() = 0;
};

#endif
