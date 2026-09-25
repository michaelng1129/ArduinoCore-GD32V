#include "Arduino.h"
#include "wiring_private.h"
#include "gd32vw55x_rcu.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    if (pin >= g_pinMapSize) {
        return;
    }

    arduinoDisablePwm(pin);

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    const uint32_t rcu = getRcuPeriph(pin);
    if (port == 0U || bit == 0U || rcu == 0U) {
        return;
    }

    rcu_periph_clock_enable(rcu);

    if (mode == OUTPUT) {
        gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, bit);
        gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, bit);
    } else if (mode == OUTPUT_OPENDRAIN) {
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

void digitalWrite(uint8_t pin, uint8_t value)
{
    if (pin >= g_pinMapSize) {
        return;
    }

    arduinoDisablePwm(pin);

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    if (port == 0U || bit == 0U) {
        return;
    }

    if (value == LOW) {
        gpio_bit_reset(port, bit);
    } else {
        gpio_bit_set(port, bit);
    }
}

int digitalRead(uint8_t pin)
{
    if (pin >= g_pinMapSize) {
        return LOW;
    }

    arduinoDisablePwm(pin);

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    if (port == 0U || bit == 0U) {
        return LOW;
    }

    return gpio_input_bit_get(port, bit) ? HIGH : LOW;
}
