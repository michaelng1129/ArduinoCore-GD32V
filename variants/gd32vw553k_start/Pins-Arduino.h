#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

// GD32VW553K-START - 21 GPIOs, QFN32
// Pin mapping based on GD32VW55x datasheet, START board schematics
// All comments in English as requested

#define NUM_DIGITAL_PINS            21
#define NUM_ANALOG_INPUTS           7
#define NUM_ANALOG_OUTPUTS          0

// LEDs
#define LED_BUILTIN                 2
#define LED_GREEN                   2

// Onboard button
#define PIN_BUTTON                  0

// UART - default Serial (PA8/PA9 for START board UART0)
#define PIN_SERIAL_RX               9   // PA9
#define PIN_SERIAL_TX               8   // PA8
#define PIN_SERIAL1_RX              9
#define PIN_SERIAL1_TX              8

// SPI - SPI0 on PA4-PA7
#define PIN_SPI_MISO                6   // PA6
#define PIN_SPI_MOSI                7   // PA7
#define PIN_SPI_SCK                 5   // PA5
#define PIN_SPI_SS                  4   // PA4

// I2C - I2C0 on PB6/PB7
#define PIN_WIRE_SDA                12  // PB6
#define PIN_WIRE_SCL                13  // PB7

// Analog pins - PA0-PA3, PB0-PB1, etc
#define PIN_A0                      0   // PA0
#define PIN_A1                      1   // PA1
#define PIN_A2                      2   // PA2
#define PIN_A3                      3   // PA3
#define PIN_A4                      10  // PB0
#define PIN_A5                      11  // PB1
#define PIN_A6                      14  // PB8 - adjust per datasheet

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;

// PWM capable pins
#define digitalPinHasPWM(p)         ((p) < NUM_DIGITAL_PINS)

// Helper macros
#define digitalPinToAnalogInput(p)  ((p) < NUM_ANALOG_INPUTS ? (p) : -1)

#endif /* _PINS_ARDUINO_H_ */
