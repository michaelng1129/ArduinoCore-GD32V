// Wire.h - Arduino Wire (I2C) library for GD32VW55x.
//
// Master mode on I2C0. Default pins: SCL=PA2, SDA=PA3 (both AF4).
// Slave mode is not implemented in this version.

#ifndef _ARDUINO_WIRE_H_
#define _ARDUINO_WIRE_H_

#include <stdint.h>
#include <stddef.h>
#include "Arduino.h"
#include "Stream.h"

#define WIRE_BUFFER_LENGTH 32

class TwoWire : public Stream {
public:
    TwoWire();

    void begin();
    void begin(uint8_t address);  // slave mode: not supported, falls back to master
    void begin(int sda, int scl);
    void end();
    void setClock(uint32_t frequency);

    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stop = true);
    uint8_t endTransmission() { return endTransmission(true); }

    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stop = true);
    uint8_t requestFrom(uint8_t address, uint8_t quantity) {
        return requestFrom(address, quantity, true);
    }

    virtual size_t write(uint8_t data) override;
    virtual size_t write(const uint8_t *data, size_t quantity) override;
    virtual int available() override;
    virtual int read() override;
    virtual int peek() override;
    virtual void flush() override;

    void onReceive(void (*function)(int)) { _onReceive = function; }
    void onRequest(void (*function)(void)) { _onRequest = function; }

private:
    bool waitFlag(uint32_t flag, bool expectSet, uint32_t timeoutMs);

    bool _begun;
    uint32_t _frequency;
    uint8_t _txAddress;
    uint8_t _txBuffer[WIRE_BUFFER_LENGTH];
    uint8_t _txLength;
    uint8_t _rxBuffer[WIRE_BUFFER_LENGTH];
    uint8_t _rxLength;
    uint8_t _rxIndex;
    void (*_onReceive)(int);
    void (*_onRequest)(void);
};

extern TwoWire Wire;

#endif
