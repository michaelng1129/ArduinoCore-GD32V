#include "variant.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"

// GD32VW553K-START - 21 GPIOs mapping
// This mapping must match pins_arduino.h comments
// Arduino pin 0..20 -> PA0..PB8 etc
const PinMap g_pinMap[] = {
    {GPIOA, GPIO_PIN_0, RCU_GPIOA},  // 0 - PA0 - BUTTON/A0
    {GPIOA, GPIO_PIN_1, RCU_GPIOA},  // 1 - PA1 - A1
    {GPIOA, GPIO_PIN_2, RCU_GPIOA},  // 2 - PA2 - LED/A2
    {GPIOA, GPIO_PIN_3, RCU_GPIOA},  // 3 - PA3 - A3
    {GPIOA, GPIO_PIN_4, RCU_GPIOA},  // 4 - PA4 - SPI SS
    {GPIOA, GPIO_PIN_5, RCU_GPIOA},  // 5 - PA5 - SPI SCK
    {GPIOA, GPIO_PIN_6, RCU_GPIOA},  // 6 - PA6 - SPI MISO
    {GPIOA, GPIO_PIN_7, RCU_GPIOA},  // 7 - PA7 - SPI MOSI
    {GPIOA, GPIO_PIN_8, RCU_GPIOA},  // 8 - PA8 - UART TX
    {GPIOA, GPIO_PIN_9, RCU_GPIOA},  // 9 - PA9 - UART RX
    {GPIOB, GPIO_PIN_0, RCU_GPIOB},  // 10 - PB0 - A4
    {GPIOB, GPIO_PIN_1, RCU_GPIOB},  // 11 - PB1 - A5
    {GPIOB, GPIO_PIN_6, RCU_GPIOB},  // 12 - PB6 - SDA
    {GPIOB, GPIO_PIN_7, RCU_GPIOB},  // 13 - PB7 - SCL
    {GPIOB, GPIO_PIN_8, RCU_GPIOB},  // 14 - PB8 - A6
    {GPIOB, GPIO_PIN_9, RCU_GPIOB},  // 15 - PB9
    {GPIOB, GPIO_PIN_10, RCU_GPIOB}, // 16 - PB10
    {GPIOB, GPIO_PIN_11, RCU_GPIOB}, // 17 - PB11
    {GPIOB, GPIO_PIN_12, RCU_GPIOB}, // 18 - PB12
    {GPIOB, GPIO_PIN_13, RCU_GPIOB}, // 19 - PB13
    {GPIOB, GPIO_PIN_14, RCU_GPIOB}, // 20 - PB14
};

const uint8_t g_pinMapSize = sizeof(g_pinMap)/sizeof(g_pinMap[0]);

uint32_t getGpioPort(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].port : 0; }
uint32_t getGpioPin(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].bit : 0; }
uint32_t getRcuPeriph(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].rcu : 0; }

extern "C" void initVariant(void) {
    // Variant specific init, called from arduino task before setup()
    // Enable all GPIO clocks
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
}
