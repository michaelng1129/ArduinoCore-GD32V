#include "variant.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_timer.h"

// JLC-GD32V (LCKFB GD32VW553 core board) - 24 digital pins
// Arduino pin numbering follows the physical header layout (see pins_arduino.h).
// Right header 0..13, left header 14..23.
const PinMap g_pinMap[] = {
    {GPIOA, GPIO_PIN_1,  RCU_GPIOA},  // 0  - PA1  - right header (A1, ADC_IN1)
    {GPIOA, GPIO_PIN_2,  RCU_GPIOA},  // 1  - PA2  - right header (A2, ADC_IN2, I2C0_SCL)
    {GPIOA, GPIO_PIN_3,  RCU_GPIOA},  // 2  - PA3  - right header (A3, ADC_IN3, I2C0_SDA)
    {GPIOA, GPIO_PIN_4,  RCU_GPIOA},  // 3  - PA4  - right header (A4, ADC_IN4, SPI_NSS)
    {GPIOA, GPIO_PIN_5,  RCU_GPIOA},  // 4  - PA5  - right header (A5, ADC_IN5, SPI_SCK)
    {GPIOA, GPIO_PIN_0,  RCU_GPIOA},  // 5  - PA0  - right header (A0, ADC_IN0, TIMER1_CH0)
    {GPIOA, GPIO_PIN_6,  RCU_GPIOA},  // 6  - PA6  - right header (A6, ADC_IN6, SPI_MISO)
    {GPIOA, GPIO_PIN_7,  RCU_GPIOA},  // 7  - PA7  - right header (A7, ADC_IN7, SPI_MOSI)
    {GPIOB, GPIO_PIN_0,  RCU_GPIOB},  // 8  - PB0  - right header (A8, ADC_IN8)
    {GPIOB, GPIO_PIN_1,  RCU_GPIOB},  // 9  - PB1  - right header (BOOT1, use with care)
    {GPIOB, GPIO_PIN_2,  RCU_GPIOB},  // 10 - PB2  - right header
    {GPIOB, GPIO_PIN_11, RCU_GPIOB},  // 11 - PB11 - right header
    {GPIOB, GPIO_PIN_12, RCU_GPIOB},  // 12 - PB12 - right header
    {GPIOB, GPIO_PIN_13, RCU_GPIOB},  // 13 - PB13 - right header
    {GPIOC, GPIO_PIN_13, RCU_GPIOC},  // 14 - PC13 - left header (LED_BUILTIN, UNCONFIRMED)
    {GPIOA, GPIO_PIN_15, RCU_GPIOA},  // 15 - PA15 - left header (JTAG JTDI)
    {GPIOB, GPIO_PIN_4,  RCU_GPIOB},  // 16 - PB4  - left header (JTAG JNTRST)
    {GPIOB, GPIO_PIN_3,  RCU_GPIOB},  // 17 - PB3  - left header (JTAG JTDO)
    {GPIOA, GPIO_PIN_12, RCU_GPIOA},  // 18 - PA12 - left header
    {GPIOA, GPIO_PIN_11, RCU_GPIOA},  // 19 - PA11 - left header
    {GPIOA, GPIO_PIN_10, RCU_GPIOA},  // 20 - PA10 - left header (USART0_RX, Serial)
    {GPIOA, GPIO_PIN_9,  RCU_GPIOA},  // 21 - PA9  - left header (USART0_TX, Serial)
    {GPIOA, GPIO_PIN_8,  RCU_GPIOA},  // 22 - PA8  - left header
    {GPIOB, GPIO_PIN_15, RCU_GPIOB},  // 23 - PB15 - left header
};

const uint8_t g_pinMapSize = sizeof(g_pinMap)/sizeof(g_pinMap[0]);

uint32_t getGpioPort(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].port : 0; }
uint32_t getGpioPin(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].bit : 0; }
uint32_t getRcuPeriph(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].rcu : 0; }

// ADC channel for a digital pin, or -1 if the pin has no ADC input.
// PA0..PA7 -> ADC_IN0..ADC_IN7, PB0 -> ADC_IN8
int getAdcChannel(uint8_t pin)
{
    switch (pin) {
        case 5:  return 0;   // PA0
        case 0:  return 1;   // PA1
        case 1:  return 2;   // PA2
        case 2:  return 3;   // PA3
        case 3:  return 4;   // PA4
        case 4:  return 5;   // PA5
        case 6:  return 6;   // PA6
        case 7:  return 7;   // PA7
        case 8:  return 8;   // PB0
        default: return -1;
    }
}

bool getPwmPinMap(uint8_t pin, PwmPinMap *map)
{
    // Only PA0 (Arduino pin 5) is routed for PWM:
    // TIMER1_CH0, alternate function AF1.
    if (pin != 5 || map == nullptr) {
        return false;
    }

    map->timer = TIMER1;
    map->timerRcu = RCU_TIMER1;
    map->period = 999U;
    map->channel = TIMER_CH_0;
    map->prescaler = 159U;
    map->alternateFunction = GPIO_AF_1;
    return true;
}

extern "C" void initVariant(void) {
    // Variant specific init, called from arduino task before setup()
    // Enable all GPIO clocks used by this variant
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
}
