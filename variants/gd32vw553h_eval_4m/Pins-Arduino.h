#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

// GD32VW553H-START - 28 GPIOs, QFN40
// Pin mapping based on GD32VW55x datasheet, START board schematics
// All comments in English as requested

#define NUM_DIGITAL_PINS            28
#define NUM_ANALOG_INPUTS           9
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
#define PIN_WIRE_SDA                19  // PB6 - H version has more pins
#define PIN_WIRE_SCL                20  // PB7

// Analog pins - PA0-PA3, PB0-PB1, PC0-PC2, etc - H has extra
#define PIN_A0                      0   // PA0
#define PIN_A1                      1   // PA1
#define PIN_A2                      2   // PA2
#define PIN_A3                      3   // PA3
#define PIN_A4                      10  // PB0
#define PIN_A5                      11  // PB1
#define PIN_A6                      21  // PC0
#define PIN_A7                      22  // PC1
#define PIN_A8                      23  // PC2

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;

// PWM capable pins
#define digitalPinHasPWM(p)         ((p) < NUM_DIGITAL_PINS)

#define digitalPinToAnalogInput(p)  ((p) < NUM_ANALOG_INPUTS ? (p) : -1)

#endif /* _PINS_ARDUINO_H_ */
