#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

// GD32VW553K-START (START V5.0 baseboard + GD32VW553-MINI-I/E module)
// Pin mapping based on:
//   - AN154 GD32VW553 Quick Development Guide (Rev 1.3b), Table 1-1/1-2
//   - GD32VW553-MINI Datasheet Rev 1.0, Table 4-1 (pin definitions)
//   - J1/J2 silkscreen order verified against the NuttX gd32vw553k-start port
//
// Digital pins 0..18 are the GPIOs broken out on the J1/J2 test headers.
// Digital pins 19..21 are the three on-board user LEDs (GPIOC, active HIGH,
// driven push-pull; named LED_RUN/LED_SLEEP/LED_RX in the vendor SDK).
// They are NOT on J1/J2, but are exposed as digital pins so LED_BUILTIN works.
//
// Boot pins PC8/BOOT0 and PB1/BOOT1 are intentionally NOT in this map:
// driving them as GPIO can change the boot mode, so they are left alone.
// JTAG pins PA13/PA14/PA15/PB3/PB4 are on J2 but shared with the on-board
// GD-Link probe through the J4 shorting caps. They work as plain GPIO only
// after removing those caps; otherwise the probe and the sketch fight over
// the lines.

#define NUM_DIGITAL_PINS            22
#define NUM_ANALOG_INPUTS           6
#define NUM_ANALOG_OUTPUTS          1

// ---------------------------------------------------------------------------
// J1 header (2x8) - user GPIO, +5V, GND
// ---------------------------------------------------------------------------
//  0 - PA0  (A0, ADC_IN0, TIMER1_CH0, SPI_MOSI)
//  1 - PA1  (A1, ADC_IN1, TIMER1_CH1, SPI_MISO)
//  2 - PA2  (A2, ADC_IN2, TIMER1_CH2, I2C0_SCL, SPI_SCK)
//  3 - PA3  (A3, ADC_IN3, TIMER1_CH3, I2C0_SDA, SPI_NSS)
//  4 - PA4  (A4, ADC_IN4, SPI_MOSI/NSS)
//  5 - PA5  (A5, ADC_IN5, SPI_MISO/SCK)
//  6 - PA6  (ADC_IN6, UART2_TX - Serial console)
//  7 - PA7  (ADC_IN7, UART2_RX - Serial console)
//  8 - PB0  (ADC_IN8, I2C0_SCL)
//  9 - PB15
//
// ---------------------------------------------------------------------------
// J2 header (2x8) - user GPIO (shared with JTAG), +3V3, GND
// ---------------------------------------------------------------------------
// 10 - PA8
// 11 - PA12
// 12 - PA13 (JTAG JTMS, shared with GD-Link)
// 13 - PA14 (JTAG JTCK, shared with GD-Link)
// 14 - PA15 (JTAG JTDI, shared with GD-Link)
// 15 - PB3  (JTAG JTDO, shared with GD-Link)
// 16 - PB4  (JTAG JNTRST, shared with GD-Link)
// 17 - PC14
// 18 - PC15
//
// ---------------------------------------------------------------------------
// On-board LEDs (not on J1/J2)
// ---------------------------------------------------------------------------
// 19 - PC0 (LED1, LED_RUN in vendor SDK)
// 20 - PC1 (LED2, LED_SLEEP in vendor SDK)
// 21 - PC2 (LED3, LED_RX in vendor SDK)

// LEDs - active HIGH, push-pull
#define LED_BUILTIN                 19
#define LED1                        19
#define LED2                        20
#define LED3                        21

// The START board has no user button: SW1 is NRST (reset) and SW2 is
// reserved per AN154. PIN_BUTTON is intentionally not defined.

// Serial - the START USB console is UART2 on PA6 (TX) / PA7 (RX),
// wired to the on-board GD-Link USB serial (AN154 Figure 1-2).
// NOTE: the vendor SDK log output (LOG_UART) also uses UART2, so SDK
// log messages and Serial output share the same USB serial port.
#define PIN_SERIAL_TX               6   // PA6 (UART2_TX)
#define PIN_SERIAL_RX               7   // PA7 (UART2_RX)
#define PIN_SERIAL1_TX              PIN_SERIAL_TX
#define PIN_SERIAL1_RX              PIN_SERIAL_RX

// SPI0 default pins (GD32VW553-MINI Datasheet Table 4-1).
// The Arduino SPI library is not implemented yet; these defines document
// the intended default routing for when it lands.
#define PIN_SPI_MOSI                0   // PA0
#define PIN_SPI_MISO                1   // PA1
#define PIN_SPI_SCK                 2   // PA2
#define PIN_SPI_SS                  4   // PA4

// I2C0 default pins (GD32VW553-MINI Datasheet Table 4-1).
// The Arduino Wire library is not implemented yet; these defines document
// the intended default routing for when it lands.
#define PIN_WIRE_SDA                3   // PA3
#define PIN_WIRE_SCL                2   // PA2

// Analog pins - PA0..PA5 are ADC_IN0..ADC_IN5 (GD32VW553-MINI Datasheet).
// PA6/PA7 (ADC_IN6/ADC_IN7) and PB0 (ADC_IN8) are also ADC-capable, but
// PA6/PA7 are the Serial console pins, so only A0..A5 get Ax aliases.
#define PIN_A0                      0   // PA0 (ADC_IN0)
#define PIN_A1                      1   // PA1 (ADC_IN1)
#define PIN_A2                      2   // PA2 (ADC_IN2)
#define PIN_A3                      3   // PA3 (ADC_IN3)
#define PIN_A4                      4   // PA4 (ADC_IN4)
#define PIN_A5                      5   // PA5 (ADC_IN5)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;

// PWM - only PA0 (TIMER1_CH0, AF1) is routed for analogWrite on START.
#define digitalPinHasPWM(p)         ((p) == 0)

// Map a digital pin number to its ADC channel, or -1 if not ADC-capable.
// PA0..PA7 -> ADC_IN0..ADC_IN7, PB0 -> ADC_IN8.
#define digitalPinToAnalogInput(p)  (((p) <= 7) ? (p) : (((p) == 8) ? 8 : -1))

#endif /* _PINS_ARDUINO_H_ */
