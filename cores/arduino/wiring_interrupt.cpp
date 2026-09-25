#include "Arduino.h"
#include "gd32vw55x_eclic.h"
#include "gd32vw55x_exti.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_syscfg.h"

#ifndef TUYAOS_SUPPORT

static voidFuncPtr g_interruptCallbacks[16] = {};
static voidFuncPtrParam g_interruptParamCallbacks[16] = {};
static void *g_interruptParameters[16] = {};

static bool getInterruptDetails(uint8_t pin, uint8_t *line, uint8_t *portSource)
{
    if (pin >= g_pinMapSize || line == nullptr || portSource == nullptr) {
        return false;
    }

    const uint32_t port = getGpioPort(pin);
    const uint32_t bit = getGpioPin(pin);
    uint8_t lineNumber = 0U;
    while (lineNumber < 16U && bit != (1UL << lineNumber)) {
        ++lineNumber;
    }
    if (lineNumber >= 16U || (lineNumber >= 5U && lineNumber <= 9U)) {
        return false;
    }

    if (port == GPIOA) {
        *portSource = EXTI_SOURCE_GPIOA;
    } else if (port == GPIOB) {
        *portSource = EXTI_SOURCE_GPIOB;
    } else if (port == GPIOC) {
        *portSource = EXTI_SOURCE_GPIOC;
    } else {
        return false;
    }

    *line = lineNumber;
    return true;
}

static void dispatchInterrupt(uint8_t line)
{
    const exti_line_enum extiLine = (exti_line_enum)(1UL << line);
    if (exti_interrupt_flag_get(extiLine) == RESET) {
        return;
    }

    exti_interrupt_flag_clear(extiLine);
    if (g_interruptParamCallbacks[line] != nullptr) {
        g_interruptParamCallbacks[line](g_interruptParameters[line]);
    } else if (g_interruptCallbacks[line] != nullptr) {
        g_interruptCallbacks[line]();
    }
}

static bool configureInterrupt(uint8_t pin, uint8_t line,
        uint8_t portSource, uint8_t mode)
{
    exti_trig_type_enum trigger;
    if (mode == CHANGE) {
        trigger = EXTI_TRIG_BOTH;
    } else if (mode == FALLING) {
        trigger = EXTI_TRIG_FALLING;
    } else if (mode == RISING) {
        trigger = EXTI_TRIG_RISING;
    } else {
        return false;
    }

    const uint32_t gpioRcu = getRcuPeriph(pin);
    if (gpioRcu == 0U) {
        return false;
    }

    rcu_periph_clock_enable(gpioRcu);
    rcu_periph_clock_enable(RCU_SYSCFG);
    syscfg_exti_line_config(portSource, line);
    exti_init((exti_line_enum)(1UL << line), EXTI_INTERRUPT, trigger);
    exti_interrupt_flag_clear((exti_line_enum)(1UL << line));
    exti_interrupt_enable((exti_line_enum)(1UL << line));

    if (line <= 4U) {
        eclic_irq_enable((IRQn_Type)(EXTI0_IRQn + line), 9U, 0U);
    } else {
        eclic_irq_enable(EXTI10_15_IRQn, 9U, 0U);
    }
    return true;
}

extern "C" int digitalPinToInterrupt(uint8_t pin)
{
    uint8_t line = 0U;
    uint8_t portSource = 0U;
    return getInterruptDetails(pin, &line, &portSource) ? (int)pin : NOT_AN_INTERRUPT;
}

extern "C" void attachInterruptParam(int interruptNumber,
        voidFuncPtrParam callback, uint8_t mode, void *parameter)
{
    if (callback == nullptr) {
        detachInterrupt(interruptNumber);
        return;
    }
    if (interruptNumber < 0 || interruptNumber >= g_pinMapSize) {
        return;
    }

    uint8_t line = 0U;
    uint8_t portSource = 0U;
    if (!getInterruptDetails((uint8_t)interruptNumber, &line, &portSource)) {
        return;
    }

    g_interruptCallbacks[line] = nullptr;
    g_interruptParamCallbacks[line] = callback;
    g_interruptParameters[line] = parameter;
    if (!configureInterrupt((uint8_t)interruptNumber, line, portSource, mode)) {
        g_interruptParamCallbacks[line] = nullptr;
        g_interruptParameters[line] = nullptr;
        exti_interrupt_disable((exti_line_enum)(1UL << line));
        exti_interrupt_flag_clear((exti_line_enum)(1UL << line));
    }
}

extern "C" void attachInterrupt(int interruptNumber, voidFuncPtr callback, uint8_t mode)
{
    if (callback == nullptr) {
        detachInterrupt(interruptNumber);
        return;
    }

    if (interruptNumber < 0 || interruptNumber >= g_pinMapSize) {
        return;
    }

    uint8_t line = 0U;
    uint8_t portSource = 0U;
    if (!getInterruptDetails((uint8_t)interruptNumber, &line, &portSource)) {
        return;
    }

    g_interruptCallbacks[line] = callback;
    g_interruptParamCallbacks[line] = nullptr;
    g_interruptParameters[line] = nullptr;
    if (!configureInterrupt((uint8_t)interruptNumber, line, portSource, mode)) {
        g_interruptCallbacks[line] = nullptr;
        exti_interrupt_disable((exti_line_enum)(1UL << line));
        exti_interrupt_flag_clear((exti_line_enum)(1UL << line));
    }
}

extern "C" void detachInterrupt(int interruptNumber)
{
    if (interruptNumber < 0 || interruptNumber >= g_pinMapSize) {
        return;
    }

    uint8_t line = 0U;
    uint8_t portSource = 0U;
    if (!getInterruptDetails((uint8_t)interruptNumber, &line, &portSource)) {
        return;
    }

    g_interruptCallbacks[line] = nullptr;
    g_interruptParamCallbacks[line] = nullptr;
    g_interruptParameters[line] = nullptr;
    exti_interrupt_disable((exti_line_enum)(1UL << line));
    exti_interrupt_flag_clear((exti_line_enum)(1UL << line));
}

static void dispatchDedicatedInterrupt(uint8_t line)
{
    sys_int_enter();
    dispatchInterrupt(line);
    sys_int_exit();
}

extern "C" void EXTI0_IRQHandler(void) { dispatchDedicatedInterrupt(0U); }
extern "C" void EXTI1_IRQHandler(void) { dispatchDedicatedInterrupt(1U); }
extern "C" void EXTI2_IRQHandler(void) { dispatchDedicatedInterrupt(2U); }
extern "C" void EXTI3_IRQHandler(void) { dispatchDedicatedInterrupt(3U); }
extern "C" void EXTI4_IRQHandler(void) { dispatchDedicatedInterrupt(4U); }

extern "C" void EXTI10_15_IRQHandler(void)
{
    sys_int_enter();
    for (uint8_t line = 10U; line < 16U; ++line) {
        dispatchInterrupt(line);
    }
    sys_int_exit();
}

#else

extern "C" int digitalPinToInterrupt(uint8_t pin)
{
    (void)pin;
    return NOT_AN_INTERRUPT;
}

extern "C" void attachInterruptParam(int interruptNumber,
        voidFuncPtrParam callback, uint8_t mode, void *parameter)
{
    (void)interruptNumber;
    (void)callback;
    (void)mode;
    (void)parameter;
}

extern "C" void attachInterrupt(int interruptNumber, voidFuncPtr callback, uint8_t mode)
{
    (void)interruptNumber;
    (void)callback;
    (void)mode;
}

extern "C" void detachInterrupt(int interruptNumber)
{
    (void)interruptNumber;
}

#endif
