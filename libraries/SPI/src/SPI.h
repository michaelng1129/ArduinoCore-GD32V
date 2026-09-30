// SPI.h - Arduino SPI library for GD32VW55x.
//
// The GD32VW553 has a single SPI peripheral. All SPI pins use AF5
// (verified against the datasheet AF mapping table).

#ifndef _ARDUINO_SPI_H_
#define _ARDUINO_SPI_H_

#include <stdint.h>
#include <stddef.h>
#include "Arduino.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

class SPISettings {
public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
        : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
    SPISettings() : _clock(1000000), _bitOrder(MSBFIRST), _dataMode(SPI_MODE0) {}

    uint32_t _clock;
    uint8_t _bitOrder;
    uint8_t _dataMode;
};

class SPIClass {
public:
    SPIClass();

    // Default pins from the variant (PIN_SPI_SCK/MISO/MOSI).
    void begin();
    // Custom pins. All SPI-capable pins use AF5 on this chip.
    void begin(int sck, int miso, int mosi, int ss = -1);
    void end();

    void beginTransaction(SPISettings settings);
    void endTransaction();

    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);

    // Legacy API
    void setClockDivider(uint8_t divider);
    void setDataMode(uint8_t dataMode);
    void setBitOrder(uint8_t bitOrder);

private:
    void configurePins(int sck, int miso, int mosi);
    void applySettings(const SPISettings &settings);

    bool _begun;
    SPISettings _settings;
    uint8_t _divider;  // legacy clock divider override, 0 = use settings
};

extern SPIClass SPI;

#endif
