#ifndef _HARDWARE_SERIAL_H_
#define _HARDWARE_SERIAL_H_

#include "Stream.h"

// Serial frame format codes, AVR-compatible bit-field encoding:
//   bits 1-2: data bits (0=5, 1=6, 2=7, 3=8)
//   bit 3:    stop bits (0=1, 1=2)
//   bits 4-5: parity (0=none, 2=even, 3=odd)
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream {
public:
    HardwareSerial();
    void begin(unsigned long baud, uint8_t config = SERIAL_8N1);
    void end();
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    int availableForWrite() override;
    operator bool();
    size_t write(uint8_t value) override;
    size_t write(const uint8_t *buffer, size_t size) override;
    using Print::write;
    // Stream::timedRead/timedPeek use the virtuals above; nothing else needed.
};

extern HardwareSerial Serial;

#endif
