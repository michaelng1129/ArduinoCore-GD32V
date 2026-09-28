#include "variant.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_timer.h"

// GD32VW553K-START - 22 digital pins
// Arduino pin 0..18  -> J1/J2 header GPIOs (see pins_arduino.h)
// Arduino pin 19..21 -> on-board LEDs PC0/PC1/PC2
const PinMap g_pinMap[] = {
    {GPIOA, GPIO_PIN_0,  RCU_GPIOA},  // 0  - PA0  - J1 (A0, TIMER1_CH0, SPI_MOSI)
    {GPIOA, GPIO_PIN_1,  RCU_GPIOA},  // 1  - PA1  - J1 (A1, SPI_MISO)
    {GPIOA, GPIO_PIN_2,  RCU_GPIOA},  // 2  - PA2  - J1 (A2, I2C0_SCL, SPI_SCK)
    {GPIOA, GPIO_PIN_3,  RCU_GPIOA},  // 3  - PA3  - J1 (A3, I2C0_SDA, SPI_NSS)
    {GPIOA, GPIO_PIN_4,  RCU_GPIOA},  // 4  - PA4  - J1 (A4, SPI_MOSI/NSS)
    {GPIOA, GPIO_PIN_5,  RCU_GPIOA},  // 5  - PA5  - J1 (A5, SPI_MISO/SCK)
    {GPIOA, GPIO_PIN_6,  RCU_GPIOA},  // 6  - PA6  - J1 (UART2_TX, Serial)
    {GPIOA, GPIO_PIN_7,  RCU_GPIOA},  // 7  - PA7  - J1 (UART2_RX, Serial)
    {GPIOB, GPIO_PIN_0,  RCU_GPIOB},  // 8  - PB0  - J1 (ADC_IN8)
    {GPIOB, GPIO_PIN_15, RCU_GPIOB},  // 9  - PB15 - J1
    {GPIOA, GPIO_PIN_8,  RCU_GPIOA},  // 10 - PA8  - J2
    {GPIOA, GPIO_PIN_12, RCU_GPIOA},  // 11 - PA12 - J2
    {GPIOA, GPIO_PIN_13, RCU_GPIOA},  // 12 - PA13 - J2 (JTAG JTMS, shared with GD-Link)
    {GPIOA, GPIO_PIN_14, RCU_GPIOA},  // 13 - PA14 - J2 (JTAG JTCK, shared with GD-Link)
    {GPIOA, GPIO_PIN_15, RCU_GPIOA},  // 14 - PA15 - J2 (JTAG JTDI, shared with GD-Link)
    {GPIOB, GPIO_PIN_3,  RCU_GPIOB},  // 15 - PB3  - J2 (JTAG JTDO, shared with GD-Link)
    {GPIOB, GPIO_PIN_4,  RCU_GPIOB},  // 16 - PB4  - J2 (JTAG JNTRST, shared with GD-Link)
    {GPIOC, GPIO_PIN_14, RCU_GPIOC},  // 17 - PC14 - J2
    {GPIOC, GPIO_PIN_15, RCU_GPIOC},  // 18 - PC15 - J2
    {GPIOC, GPIO_PIN_0,  RCU_GPIOC},  // 19 - PC0  - LED1 (LED_BUILTIN)
    {GPIOC, GPIO_PIN_1,  RCU_GPIOC},  // 20 - PC1  - LED2
    {GPIOC, GPIO_PIN_2,  RCU_GPIOC},  // 21 - PC2  - LED3
};

const uint8_t g_pinMapSize = sizeof(g_pinMap)/sizeof(g_pinMap[0]);

uint32_t getGpioPort(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].port : 0; }
uint32_t getGpioPin(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].bit : 0; }
uint32_t getRcuPeriph(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].rcu : 0; }

// ADC channel for a digital pin, or -1 if the pin has no ADC input.
// PA0..PA7 -> ADC_IN0..ADC_IN7, PB0 -> ADC_IN8
// (GD32VW553-MINI Datasheet Table 4-1).
int getAdcChannel(uint8_t pin)
{
    if (pin <= 7) {
        return pin;
    }
    if (pin == 8) {
        return 8;
    }
    return -1;
}

bool getPwmPinMap(uint8_t pin, PwmPinMap *map)
{
    // Only PA0 (Arduino pin 0) is routed for PWM on START:
    // TIMER1_CH0, alternate function AF1
    // (GD32VW553-MINI Datasheet Table 4-1).
    if (pin != 0 || map == nullptr) {
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
