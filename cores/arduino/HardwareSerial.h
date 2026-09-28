#ifndef _HARDWARE_SERIAL_H_
#define _HARDWARE_SERIAL_H_

#include "Stream.h"

class HardwareSerial : public Stream {
public:
    HardwareSerial();
    void begin(unsigned long baud);
    void end();
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    size_t write(uint8_t value) override;
    using Print::write;
    // Stream::timedRead/timedPeek use the virtuals above; nothing else needed.
};

extern HardwareSerial Serial;

#endif
