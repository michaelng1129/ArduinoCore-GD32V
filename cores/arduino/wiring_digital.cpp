#include "Arduino.h"
#include "gd32vw55x_rcu.h"

void pinMode(uint8_t pin, uint8_t mode) {
    if (pin >= g_pinMapSize) return;
    uint32_t port = getGpioPort(pin);
    uint32_t bit = getGpioPin(pin);
    uint32_t rcu = getRcuPeriph(pin);
    if (!port) return;
    rcu_periph_clock_enable(rcu);
    if (mode == OUTPUT || mode == OUTPUT_PP) {
        gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, bit);
        gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, bit);
    } else if (mode == OUTPUT_OD) {
        gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, bit);
        gpio_output_options_set(port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, bit);
    } else if (mode == INPUT_PULLUP) {
        gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, bit);
    } else if (mode == INPUT_PULLDOWN) {
        gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, bit);
    } else {
        gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, bit);
    }
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin >= g_pinMapSize) return;
    uint32_t port = getGpioPort(pin);
    uint32_t bit = getGpioPin(pin);
    if (!port) return;
    if (val == HIGH) gpio_bit_set(port, bit);
    else gpio_bit_reset(port, bit);
}

int digitalRead(uint8_t pin) {
    if (pin >= g_pinMapSize) return LOW;
    uint32_t port = getGpioPort(pin);
    uint32_t bit = getGpioPin(pin);
    if (!port) return LOW;
    return gpio_input_bit_get(port, bit) ? HIGH : LOW;
}
