#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

// GD32VW553K-START - 21 GPIOs, QFN32
// Pin mapping based on GD32VW55x datasheet, START board schematics
// All comments in English as requested

#define NUM_DIGITAL_PINS            21
#define NUM_ANALOG_INPUTS           9
#define NUM_ANALOG_OUTPUTS          1

// LEDs
#define LED_BUILTIN                 2
#define LED_GREEN                   2

// Onboard button
#define PIN_BUTTON                  0

// UART - default Serial uses USART0 on PA0/PA1.
#define PIN_SERIAL_RX               1   // PA1 (USART0 RX)
#define PIN_SERIAL_TX               0   // PA0 (USART0 TX)
#define PIN_SERIAL1_RX              PIN_SERIAL_RX
#define PIN_SERIAL1_TX              PIN_SERIAL_TX

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
#define PIN_A4                      4   // PA4
#define PIN_A5                      5   // PA5
#define PIN_A6                      6   // PA6
#define PIN_A7                      7   // PA7
#define PIN_A8                      10  // PB0 (ADC_IN8)

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
#define digitalPinHasPWM(p)         ((p) == 0)  // PA0: TIMER1_CH0

// Helper macros
#define digitalPinToAnalogInput(p)  ((p) < 8 ? (p) : ((p) == 10 ? 8 : -1))

#endif /* _PINS_ARDUINO_H_ */
