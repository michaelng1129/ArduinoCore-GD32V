#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32vw55x.h"
#include "gd32vw55x_gpio.h"
#include "wrapper_os.h"
#include "pins_arduino.h"
#include "variant.h"

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;
typedef uint8_t PinMode;
typedef uint8_t PinStatus;
typedef uint8_t BitOrder;
typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrParam)(void *);

#define HIGH                0x1
#define LOW                 0x0
#define INPUT               0x0
#define OUTPUT              0x1
#define INPUT_PULLUP        0x2
#define INPUT_PULLDOWN      0x3
#define OUTPUT_OPENDRAIN    0x4
#define OUTPUT_PP           OUTPUT
#define OUTPUT_OD           OUTPUT_OPENDRAIN
#define CHANGE              0x2
#define FALLING             0x3
#define RISING              0x4
#define LSBFIRST            0
#define MSBFIRST            1
// This device uses a fixed ADC reference; these names preserve sketch compatibility.
#define DEFAULT             1
#define EXTERNAL            0
#define INTERNAL            DEFAULT
#define SERIAL              0
#define DISPLAY             1
#define LED_BUILTIN_ACTIVE  LOW
#define NOT_AN_INTERRUPT    (-1)

#define DEC                 10
#define HEX                 16
#define OCT                 8
#define BIN                 2

#ifndef PI
#define PI                  3.1415926535897932384626433832795
#endif
#ifndef HALF_PI
#define HALF_PI             1.5707963267948966192313216916398
#endif
#ifndef TWO_PI
#define TWO_PI              6.283185307179586476925286766559
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD          0.017453292519943295769236907684886
#endif
#ifndef RAD_TO_DEG
#define RAD_TO_DEG          57.295779513082320876798154814105
#endif
#ifndef EULER
#define EULER               2.718281828459045235360287471352
#endif

#ifndef constrain
#define constrain(value, lower, upper) \
    ((value) < (lower) ? (lower) : ((value) > (upper) ? (upper) : (value)))
#endif
#ifndef radians
#define radians(degreesValue) ((degreesValue) * DEG_TO_RAD)
#endif
#ifndef degrees
#define degrees(radiansValue) ((radiansValue) * RAD_TO_DEG)
#endif
#ifndef sq
#define sq(value) ((value) * (value))
#endif

#define lowByte(value)      ((uint8_t)((value) & 0xFF))
#define highByte(value)     ((uint8_t)(((value) >> 8) & 0xFF))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01U)
#define bitSet(value, bit)  ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitValue) \
    ((bitValue) ? bitSet((value), (bit)) : bitClear((value), (bit)))
#ifndef bit
#define bit(position)       (1UL << (position))
#endif

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
int digitalPinToInterrupt(uint8_t pin);
void attachInterrupt(int interruptNumber, voidFuncPtr callback, uint8_t mode);
void attachInterruptParam(int interruptNumber, voidFuncPtrParam callback,
        uint8_t mode, void *parameter);
void detachInterrupt(int interruptNumber);

int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogReadResolution(uint8_t bits);
void analogWrite(uint8_t pin, int value);
void analogWriteResolution(uint8_t bits);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long milliseconds);
void delayMicroseconds(unsigned int microseconds);
void yield(void);
void noInterrupts(void);
void interrupts(void);

#ifdef __cplusplus
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);
#else
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);
#endif

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void init(void);
void initVariant(void);

#ifdef __cplusplus
}

template <typename T> static inline T arduino_min(T a, T b) { return a < b ? a : b; }
template <typename T> static inline T arduino_max(T a, T b) { return a > b ? a : b; }
#ifndef min
#define min(a, b) arduino_min((a), (b))
#endif
#ifndef max
#define max(a, b) arduino_max((a), (b))
#endif

static inline word makeWord(unsigned int value) { return (word)value; }
static inline word makeWord(unsigned int high, unsigned int low)
{
    return (word)((high << 8) | low);
}

#ifndef F
#define F(stringLiteral) (stringLiteral)
#endif

#include "WMath.h"
#include "WString.h"
#include "Print.h"
#include "HardwareSerial.h"
#endif

void setup(void);
void loop(void);

#endif
