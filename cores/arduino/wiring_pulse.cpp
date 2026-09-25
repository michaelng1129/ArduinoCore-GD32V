#include "Arduino.h"

unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
    if (pin >= g_pinMapSize || (state != LOW && state != HIGH) || timeout == 0UL) {
        return 0UL;
    }

    const unsigned long waitStart = micros();

    while (digitalRead(pin) == state) {
        if ((unsigned long)(micros() - waitStart) >= timeout) {
            return 0UL;
        }
    }

    while (digitalRead(pin) != state) {
        if ((unsigned long)(micros() - waitStart) >= timeout) {
            return 0UL;
        }
    }

    const unsigned long pulseStart = micros();
    while (digitalRead(pin) == state) {
        if ((unsigned long)(micros() - waitStart) >= timeout) {
            return 0UL;
        }
    }

    return (unsigned long)(micros() - pulseStart);
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return pulseInLong(pin, state, timeout);
}
