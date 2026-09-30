// wiring_tone.cpp - tone()/noTone() for GD32VW55x
//
// Generates a square wave on a PWM-capable pin using the pin's timer
// channel at 50% duty cycle. The timer clock is assumed to be CK_SYS
// (APB prescalers are 1 on the default 160MHz configuration).

#include "Arduino.h"
#include "wiring_private.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_timer.h"
#include "FreeRTOS.h"
#include "timers.h"

static int8_t g_tonePin = -1;
static TimerHandle_t g_toneTimer = nullptr;

static void toneTimerCallback(TimerHandle_t timer)
{
    (void)timer;
    if (g_tonePin >= 0) {
        noTone((uint8_t)g_tonePin);
    }
}

static void stopToneTimer(void)
{
    if (g_toneTimer != nullptr) {
        xTimerStop(g_toneTimer, 0);
        xTimerDelete(g_toneTimer, 0);
        g_toneTimer = nullptr;
    }
}

void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    if (pin >= g_pinMapSize || frequency == 0U) {
        return;
    }

    PwmPinMap map;
    if (!getPwmPinMap(pin, &map)) {
        return;  // Pin has no timer channel; cannot generate tone.
    }

    // Stop any previous tone first.
    noTone(pin);

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    const uint32_t gpioRcu = getRcuPeriph(pin);
    if (port == 0U || bit == 0U || gpioRcu == 0U) {
        return;
    }

    // Compute prescaler for the target frequency.
    // f = timer_clk / ((prescaler+1) * (period+1)), 50% duty.
    const uint32_t timerClk = rcu_clock_freq_get(CK_SYS);
    const uint32_t period = 999U;
    uint32_t prescaler = timerClk / ((uint32_t)frequency * (period + 1U));
    if (prescaler > 0U) {
        prescaler -= 1U;
    }
    if (prescaler > 65535U) {
        prescaler = 65535U;  // Clamp; actual frequency will be higher than requested.
    }

    rcu_periph_clock_enable(map.timerRcu);
    rcu_periph_clock_enable(gpioRcu);

    timer_parameter_struct timer;
    timer_deinit(map.timer);
    timer_struct_para_init(&timer);
    timer.prescaler = (uint16_t)prescaler;
    timer.alignedmode = TIMER_COUNTER_EDGE;
    timer.counterdirection = TIMER_COUNTER_UP;
    timer.period = period;
    timer.clockdivision = TIMER_CKDIV_DIV1;
    timer.repetitioncounter = 0U;
    timer_init(map.timer, &timer);
    timer_auto_reload_shadow_enable(map.timer);

    timer_oc_parameter_struct channel;
    timer_channel_output_struct_para_init(&channel);
    channel.outputstate = TIMER_CCX_ENABLE;
    channel.outputnstate = TIMER_CCXN_DISABLE;
    channel.ocpolarity = TIMER_OC_POLARITY_HIGH;
    channel.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    channel.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    channel.ocnidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(map.timer, map.channel, &channel);
    timer_channel_output_mode_config(map.timer, map.channel, TIMER_OC_MODE_PWM0);
    timer_channel_output_pulse_value_config(map.timer, map.channel, (period + 1U) / 2U);
    timer_channel_output_shadow_config(map.timer, map.channel, TIMER_OC_SHADOW_DISABLE);

    gpio_af_set(port, map.alternateFunction, bit);
    gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, bit);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, bit);
    timer_channel_output_state_config(map.timer, map.channel, TIMER_CCX_ENABLE);
    timer_enable(map.timer);

    g_tonePin = (int8_t)pin;

    // One-shot duration: stop the tone after `duration` ms.
    if (duration > 0UL) {
        g_toneTimer = xTimerCreate("tone", pdMS_TO_TICKS(duration), pdFALSE,
                nullptr, toneTimerCallback);
        if (g_toneTimer != nullptr) {
            xTimerStart(g_toneTimer, 0);
        }
    }
}

void noTone(uint8_t pin)
{
    stopToneTimer();

    if (g_tonePin >= 0 && (uint8_t)g_tonePin != pin) {
        return;  // A different pin is toning; leave it alone.
    }
    g_tonePin = -1;

    if (pin >= g_pinMapSize) {
        return;
    }

    // Tear down the timer channel directly (do not rely on the PWM
    // module's configured-flag, which tone() never sets).
    PwmPinMap map;
    if (!getPwmPinMap(pin, &map)) {
        return;
    }
    timer_channel_output_state_config(map.timer, map.channel, TIMER_CCX_DISABLE);

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    const uint32_t rcu = getRcuPeriph(pin);
    if (port != 0U && bit != 0U && rcu != 0U) {
        rcu_periph_clock_enable(rcu);
        gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, bit);
        gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, bit);
        gpio_bit_reset(port, bit);
    }
}
