// pins_arduino.h - JLC-GD32V (LCKFB GD32VW553 core board) pin definitions
//
// Board: 立创 LCKFB GD32VW553 核心板 (GD32VW553H, QFN40, 4MB Flash)
// Pinout derived from the vendor pinout diagram (引脚接口图).
// Serial/UART0 pins (PA8=RX, PB15=TX) and the PC13 LED are confirmed
// against the schematic. The KEY button pin is still a guess.
//
// Arduino pin numbering follows the physical header layout:
//
// Right header (top to bottom):
//   0 - PA1            8 - PB0
//   1 - PA2            9 - PB1 (BOOT1, affects boot mode - use with care)
//   2 - PA3           10 - PB2
//   3 - PA4           11 - PB11
//   4 - PA5           12 - PB12
//   5 - PA0           13 - PB13
//   6 - PA6
//   7 - PA7
//
// Left header (top to bottom, GPIO only):
//  14 - PC13 (LED)    19 - PA11
//  15 - PA15          20 - PA10
//  16 - PB4           21 - PA9
//  17 - PB3           22 - PA8  (USART0_RX)
//  18 - PA12          23 - PB15 (USART0_TX)
//
// (BOOT0, RESET, 3V3, 5V, GND are not mapped as Arduino pins.)

#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <stdint.h>

// Board identification
#define BOARD_JLC_GD32V

// --- Digital pins ---
#define NUM_DIGITAL_PINS            24
#define NUM_ANALOG_INPUTS           9

// --- LEDs ---
// Confirmed: PC13 is wired to an on-board LED (per schematic).
#define LED_BUILTIN                 14  // PC13
#define LED_BUILTIN_ACTIVE          HIGH

// --- Buttons ---
// Confirmed: SW2 (KEY) on PA0/WKUP0, active HIGH (10kΩ pulldown to GND).
#define PIN_BUTTON                  5   // PA0
#define PIN_BOOT0                   255 // PC8, not on Arduino header (BOOT0 button SW1)

// --- Serial (USB) ---
// Confirmed: the on-board USB-UART bridge is wired to USART0
// (PA8=RX, PB15=TX).
#define PIN_SERIAL_TX               23  // PB15 (USART0_TX)
#define PIN_SERIAL_RX               22  // PA8 (USART0_RX)
#define PIN_SERIAL1_TX              PIN_SERIAL_TX
#define PIN_SERIAL1_RX              PIN_SERIAL_RX

// HardwareSerial peripheral configuration (used by cores/arduino/HardwareSerial.cpp)
// USART0: TX on PB15 (AF8), RX on PA8 (AF2). Verified against the
// GD32VW553xx Datasheet AF mapping tables (pp. 24-25).
#define ARDUINO_SERIAL_USART        USART0
#define ARDUINO_SERIAL_IRQn         USART0_IRQn
#define ARDUINO_SERIAL_RCU          RCU_USART0
#define ARDUINO_SERIAL_TX_PORT      GPIOB
#define ARDUINO_SERIAL_TX_PIN       GPIO_PIN_15
#define ARDUINO_SERIAL_TX_AF        GPIO_AF_8
#define ARDUINO_SERIAL_RX_PORT      GPIOA
#define ARDUINO_SERIAL_RX_PIN       GPIO_PIN_8
#define ARDUINO_SERIAL_RX_AF        GPIO_AF_2

// --- SPI0 default pins (GD32VW553H Datasheet pinmux) ---
// The Arduino SPI library is not implemented yet; these defines document
// the intended default pins.
#define PIN_SPI_MOSI                7   // PA7
#define PIN_SPI_MISO                6   // PA6
#define PIN_SPI_SCK                 4   // PA5
#define PIN_SPI_NSS                 3   // PA4

// --- I2C0 default pins ---
// The Arduino Wire library is not implemented yet; these defines document
// the intended default pins.
#define PIN_WIRE_SDA                2   // PA3
#define PIN_WIRE_SCL                1   // PA2

// --- Analog ---
// A0..A8 map to ADC_IN0..ADC_IN8 (PA0..PA7, PB0).
#define A0                          5   // PA0 (ADC_IN0)
#define A1                          0   // PA1 (ADC_IN1)
#define A2                          1   // PA2 (ADC_IN2)
#define A3                          2   // PA3 (ADC_IN3)
#define A4                          3   // PA4 (ADC_IN4)
#define A5                          4   // PA5 (ADC_IN5)
#define A6                          6   // PA6 (ADC_IN6)
#define A7                          7   // PA7 (ADC_IN7)
#define A8                          8   // PB0 (ADC_IN8)

// --- PWM ---
// Currently only PA0 (TIMER1_CH0, AF1) is mapped. The H chip has many more
// timer channels; extend getPwmConfig() in variant.cpp as needed.
#define PIN_PWM_DEFAULT             5   // PA0

// --- Internal helpers (used by variant.cpp) ---
#define PIN_GPIOA_0                 5
#define PIN_GPIOA_1                 0
#define PIN_GPIOA_2                 1
#define PIN_GPIOA_3                 2
#define PIN_GPIOA_4                 3
#define PIN_GPIOA_5                 4
#define PIN_GPIOA_6                 6
#define PIN_GPIOA_7                 7
#define PIN_GPIOA_8                 22
#define PIN_GPIOA_9                 21
#define PIN_GPIOA_10                20
#define PIN_GPIOA_11                19
#define PIN_GPIOA_12                18
#define PIN_GPIOA_15                15
#define PIN_GPIOB_0                 8
#define PIN_GPIOB_1                 9
#define PIN_GPIOB_2                 10
#define PIN_GPIOB_3                 17
#define PIN_GPIOB_4                 16
#define PIN_GPIOB_11                11
#define PIN_GPIOB_12                12
#define PIN_GPIOB_13                13
#define PIN_GPIOB_15                23
#define PIN_GPIOC_13                14

#endif // _PINS_ARDUINO_H_
