#ifndef _HARDWARE_SERIAL_H_
#define _HARDWARE_SERIAL_H_

#include "Print.h"

class HardwareSerial : public Print {
public:
    HardwareSerial();
    void begin(unsigned long baud);
    void end();
    int available();
    int read();
    int peek();
    void flush();
    size_t write(uint8_t value) override;
    using Print::write;
};

extern HardwareSerial Serial;

#endif
