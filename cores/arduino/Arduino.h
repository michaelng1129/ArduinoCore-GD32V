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

// SDK includes - platform already initialized in main.cpp
#include "gd32vw55x.h"
#include "gd32vw55x_gpio.h"
#include "wrapper_os.h"

// Variant pins
#include "pins_arduino.h"
#include "variant.h"

// Arduino types
#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3
#define OUTPUT_PP 0x1
#define OUTPUT_OD 0x2

// Wiring API
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis(void);
unsigned long micros(void);
void yield(void);

void init(void);
void initVariant(void);

// For C++ sketches
#ifdef __cplusplus
}
#include "WString.h"
#include "HardwareSerial.h"
#endif

// Setup/loop declared by sketch
void setup(void);
void loop(void);

#endif
