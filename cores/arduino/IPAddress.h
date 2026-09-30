// IPAddress.h - IP address helper, Arduino API compatible.
// Mirrors ArduinoCore-API's api/IPAddress.h (IPv4 only).

#ifndef _IPADDRESS_H_
#define _IPADDRESS_H_

#include <stdint.h>
#include "Printable.h"
#include "WString.h"

class IPAddress : public Printable {
private:
    union {
        uint8_t bytes[4];
        uint32_t dword;
    } _address;

public:
    IPAddress();
    IPAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);

    bool fromString(const char *address);
    bool fromString(const String &address) { return fromString(address.c_str()); }

    operator uint32_t() const { return _address.dword; }
    bool operator==(const IPAddress &addr) const { return _address.dword == addr._address.dword; }
    bool operator==(uint32_t addr) const { return _address.dword == addr; }
    bool operator!=(const IPAddress &addr) const { return !(*this == addr); }
    bool operator!=(uint32_t addr) const { return !(*this == addr); }

    uint8_t operator[](int index) const { return _address.bytes[index]; }
    uint8_t &operator[](int index) { return _address.bytes[index]; }

    String toString() const;

    virtual size_t printTo(Print &p) const override;
};

#endif
