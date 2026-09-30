// Printable.h - printable object interface, Arduino API compatible.
// Mirrors ArduinoCore-API's api/Printable.h.

#ifndef _PRINTABLE_H_
#define _PRINTABLE_H_

#include <stddef.h>

class Print;

class Printable {
public:
    virtual size_t printTo(Print &p) const = 0;
};

#endif
