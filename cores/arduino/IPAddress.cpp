// IPAddress.cpp - IPv4 address helper.

#include "Arduino.h"
#include "IPAddress.h"
#include <stdio.h>
#include <string.h>

IPAddress::IPAddress()
{
    _address.dword = 0;
}

IPAddress::IPAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    _address.bytes[0] = b0;
    _address.bytes[1] = b1;
    _address.bytes[2] = b2;
    _address.bytes[3] = b3;
}

IPAddress::IPAddress(uint32_t address)
{
    _address.dword = address;
}

IPAddress::IPAddress(const uint8_t *address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
}

bool IPAddress::fromString(const char *address)
{
    uint16_t acc = 0;
    uint8_t dots = 0;
    uint8_t b[4] = {0, 0, 0, 0};

    while (*address) {
        char c = *address++;
        if (c >= '0' && c <= '9') {
            acc = acc * 10 + (uint16_t)(c - '0');
            if (acc > 255) {
                return false;
            }
        } else if (c == '.') {
            if (dots == 3) {
                return false;
            }
            b[dots++] = (uint8_t)acc;
            acc = 0;
        } else {
            return false;
        }
    }

    if (dots != 3) {
        return false;
    }
    b[3] = (uint8_t)acc;
    memcpy(_address.bytes, b, sizeof(b));
    return true;
}

String IPAddress::toString() const
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u",
            _address.bytes[0], _address.bytes[1],
            _address.bytes[2], _address.bytes[3]);
    return String(buf);
}

size_t IPAddress::printTo(Print &p) const
{
    size_t n = 0;
    for (int i = 0; i < 3; i++) {
        n += p.print(_address.bytes[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address.bytes[3], DEC);
    return n;
}
