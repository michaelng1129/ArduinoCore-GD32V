#include "Arduino.h"

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    digitalWrite(clockPin, LOW);

    for (uint8_t bit = 0U; bit < 8U; ++bit) {
        const uint8_t valueBit = bitOrder == LSBFIRST ? bit : (uint8_t)(7U - bit);
        digitalWrite(dataPin, (value & (1U << valueBit)) ? HIGH : LOW);
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint8_t value = 0U;
    pinMode(dataPin, INPUT);
    pinMode(clockPin, OUTPUT);

    for (uint8_t bit = 0U; bit < 8U; ++bit) {
        digitalWrite(clockPin, HIGH);
        const uint8_t valueBit = bitOrder == LSBFIRST ? bit : (uint8_t)(7U - bit);
        if (digitalRead(dataPin) == HIGH) {
            value |= (uint8_t)(1U << valueBit);
        }
        digitalWrite(clockPin, LOW);
    }

    return value;
}
