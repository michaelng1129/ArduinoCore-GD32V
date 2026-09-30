// Server.h - TCP server interface, Arduino API compatible.
// Mirrors ArduinoCore-API's api/Server.h.

#ifndef _SERVER_H_
#define _SERVER_H_

#include "Print.h"

class Client;

class Server : public Print {
public:
    virtual void begin() = 0;
};

#endif
