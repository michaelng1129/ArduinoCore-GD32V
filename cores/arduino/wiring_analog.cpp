#include "Arduino.h"
#include "wiring_private.h"
#include "gd32vw55x_adc.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_timer.h"

static const uint8_t kHardwareAdcResolution = 12U;
static uint8_t g_analogReadResolution = kHardwareAdcResolution;
static uint8_t g_analogWriteResolution = 8U;
static bool g_adcInitialized = false;
static bool g_pwmChannelConfigured[NUM_DIGITAL_PINS] = {};
static uint32_t g_initializedPwmTimers[NUM_DIGITAL_PINS] = {};
static uint8_t g_initializedPwmTimerCount = 0U;

static uint32_t resolutionMaximum(uint8_t bits)
{
    return (1UL << bits) - 1UL;
}

static bool initializePwmTimer(const PwmPinMap *map)
{
    for (uint8_t i = 0U; i < g_initializedPwmTimerCount; ++i) {
        if (g_initializedPwmTimers[i] == map->timer) {
            return true;
        }
    }

    if (g_initializedPwmTimerCount >= NUM_DIGITAL_PINS) {
        return false;
    }

    timer_parameter_struct timer;
    rcu_periph_clock_enable(map->timerRcu);
    timer_deinit(map->timer);
    timer_struct_para_init(&timer);
    timer.prescaler = map->prescaler;
    timer.alignedmode = TIMER_COUNTER_EDGE;
    timer.counterdirection = TIMER_COUNTER_UP;
    timer.period = map->period;
    timer.clockdivision = TIMER_CKDIV_DIV1;
    timer.repetitioncounter = 0U;
    timer_init(map->timer, &timer);
    timer_auto_reload_shadow_enable(map->timer);
    timer_enable(map->timer);

    g_initializedPwmTimers[g_initializedPwmTimerCount++] = map->timer;
    return true;
}

static bool preparePwmChannel(uint8_t pin, const PwmPinMap *map)
{
    if (!initializePwmTimer(map)) {
        return false;
    }

    if (!g_pwmChannelConfigured[pin]) {
        timer_oc_parameter_struct channel;
        timer_channel_output_struct_para_init(&channel);
        channel.outputstate = TIMER_CCX_ENABLE;
        channel.outputnstate = TIMER_CCXN_DISABLE;
        channel.ocpolarity = TIMER_OC_POLARITY_HIGH;
        channel.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
        channel.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
        channel.ocnidlestate = TIMER_OC_IDLE_STATE_LOW;

        timer_channel_output_config(map->timer, map->channel, &channel);
        timer_channel_output_mode_config(map->timer, map->channel, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(map->timer, map->channel, TIMER_OC_SHADOW_DISABLE);
        g_pwmChannelConfigured[pin] = true;
    }

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    const uint32_t rcu = getRcuPeriph(pin);
    if (port == 0U || bit == 0U || rcu == 0U) {
        return false;
    }

    rcu_periph_clock_enable(rcu);
    gpio_af_set(port, map->alternateFunction, bit);
    gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, bit);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, bit);
    timer_channel_output_state_config(map->timer, map->channel, TIMER_CCX_ENABLE);
    return true;
}

void analogReference(uint8_t mode)
{
    // The GD32VW55x uses a fixed ADC reference voltage.
    (void)mode;
}

void analogReadResolution(uint8_t bits)
{
    if (bits < 1U) {
        bits = 1U;
    } else if (bits > 16U) {
        bits = 16U;
    }
    g_analogReadResolution = bits;
}

int analogRead(uint8_t pin)
{
    if (pin >= g_pinMapSize) {
        return 0;
    }

    const int channel = getAdcChannel(pin);
    if (channel < 0 || channel > 8) {
        return 0;
    }

    pinMode(pin, INPUT);
    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    const uint32_t gpioRcu = getRcuPeriph(pin);
    if (port == 0U || bit == 0U || gpioRcu == 0U) {
        return 0;
    }

    rcu_periph_clock_enable(gpioRcu);
    gpio_mode_set(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, bit);
    rcu_periph_clock_enable(RCU_ADC);

    if (!g_adcInitialized) {
        adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
        adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
        adc_channel_length_config(ADC_ROUTINE_CHANNEL, 1U);
        adc_external_trigger_config(ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
        adc_resolution_config(ADC_RESOLUTION_12B);
        adc_enable();
        g_adcInitialized = true;
    }

    adc_routine_channel_config(0U, (uint8_t)channel, ADC_SAMPLETIME_479POINT5);
    adc_flag_clear(ADC_FLAG_EOC);
    adc_software_trigger_enable(ADC_ROUTINE_CHANNEL);
    while (RESET == adc_flag_get(ADC_FLAG_EOC)) {
    }

    const uint32_t sample = adc_routine_data_read();
    const uint32_t inputMaximum = resolutionMaximum(kHardwareAdcResolution);
    const uint32_t outputMaximum = resolutionMaximum(g_analogReadResolution);
    return (int)(((uint64_t)sample * outputMaximum + inputMaximum / 2U) / inputMaximum);
}

void analogWriteResolution(uint8_t bits)
{
    if (bits < 1U) {
        bits = 1U;
    } else if (bits > 16U) {
        bits = 16U;
    }
    g_analogWriteResolution = bits;
}

void analogWrite(uint8_t pin, int value)
{
    if (pin >= g_pinMapSize) {
        return;
    }

    const uint32_t outputMaximum = resolutionMaximum(g_analogWriteResolution);
    if (value < 0) {
        value = 0;
    } else if ((uint32_t)value > outputMaximum) {
        value = (int)outputMaximum;
    }

    if (value == 0 || (uint32_t)value == outputMaximum) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, value == 0 ? LOW : HIGH);
        return;
    }

    PwmPinMap map;
    if (!getPwmPinMap(pin, &map)) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, ((uint32_t)value * 2U) >= outputMaximum ? HIGH : LOW);
        return;
    }

    if (!preparePwmChannel(pin, &map)) {
        return;
    }

    const uint32_t pulse = ((uint64_t)(uint32_t)value * map.period) / outputMaximum;
    timer_channel_output_pulse_value_config(map.timer, map.channel, pulse);
}

void arduinoDisablePwm(uint8_t pin)
{
    if (pin >= g_pinMapSize || pin >= NUM_DIGITAL_PINS || !g_pwmChannelConfigured[pin]) {
        return;
    }

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
    }
}
