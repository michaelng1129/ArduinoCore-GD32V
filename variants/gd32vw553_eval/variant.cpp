#include "variant.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_timer.h"

// GD32VW553H-START - 28 GPIOs mapping
const PinMap g_pinMap[] = {
    {GPIOA, GPIO_PIN_0, RCU_GPIOA},  // 0 - PA0
    {GPIOA, GPIO_PIN_1, RCU_GPIOA},  // 1 - PA1
    {GPIOA, GPIO_PIN_2, RCU_GPIOA},  // 2 - PA2 - LED
    {GPIOA, GPIO_PIN_3, RCU_GPIOA},  // 3 - PA3
    {GPIOA, GPIO_PIN_4, RCU_GPIOA},  // 4 - PA4
    {GPIOA, GPIO_PIN_5, RCU_GPIOA},  // 5 - PA5
    {GPIOA, GPIO_PIN_6, RCU_GPIOA},  // 6 - PA6
    {GPIOA, GPIO_PIN_7, RCU_GPIOA},  // 7 - PA7
    {GPIOA, GPIO_PIN_8, RCU_GPIOA},  // 8 - PA8 - TX
    {GPIOA, GPIO_PIN_9, RCU_GPIOA},  // 9 - PA9 - RX
    {GPIOB, GPIO_PIN_0, RCU_GPIOB},  // 10 - PB0
    {GPIOB, GPIO_PIN_1, RCU_GPIOB},  // 11 - PB1
    {GPIOB, GPIO_PIN_2, RCU_GPIOB},  // 12 - PB2
    {GPIOB, GPIO_PIN_3, RCU_GPIOB},  // 13 - PB3
    {GPIOB, GPIO_PIN_4, RCU_GPIOB},  // 14 - PB4
    {GPIOB, GPIO_PIN_5, RCU_GPIOB},  // 15 - PB5
    {GPIOB, GPIO_PIN_6, RCU_GPIOB},  // 16 - PB6 - SDA
    {GPIOB, GPIO_PIN_7, RCU_GPIOB},  // 17 - PB7 - SCL
    {GPIOB, GPIO_PIN_8, RCU_GPIOB},  // 18 - PB8
    {GPIOB, GPIO_PIN_9, RCU_GPIOB},  // 19 - PB9
    {GPIOB, GPIO_PIN_10, RCU_GPIOB}, // 20 - PB10
    {GPIOB, GPIO_PIN_11, RCU_GPIOB}, // 21 - PB11
    {GPIOC, GPIO_PIN_0, RCU_GPIOC},  // 22 - PC0 - A6
    {GPIOC, GPIO_PIN_1, RCU_GPIOC},  // 23 - PC1 - A7
    {GPIOC, GPIO_PIN_2, RCU_GPIOC},  // 24 - PC2 - A8
    {GPIOC, GPIO_PIN_3, RCU_GPIOC},  // 25 - PC3
    {GPIOC, GPIO_PIN_4, RCU_GPIOC},  // 26 - PC4
    {GPIOC, GPIO_PIN_5, RCU_GPIOC},  // 27 - PC5
};

const uint8_t g_pinMapSize = sizeof(g_pinMap)/sizeof(g_pinMap[0]);

uint32_t getGpioPort(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].port : 0; }
uint32_t getGpioPin(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].bit : 0; }
uint32_t getRcuPeriph(uint8_t pin) { return (pin < g_pinMapSize) ? g_pinMap[pin].rcu : 0; }
int getAdcChannel(uint8_t pin) { return pin < 8 ? pin : (pin == 10 ? 8 : -1); }

bool getPwmPinMap(uint8_t pin, PwmPinMap *map)
{
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
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
}
