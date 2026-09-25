#include "Arduino.h"
#include "ArduinoPlatform.h"
#include "FreeRTOS.h"
#include "task.h"
#include "core_feature_base.h"

extern "C" {

void init(void)
{
}

unsigned long millis(void)
{
    return (unsigned long)(arduinoPlatformMicros() / 1000ULL);
}

unsigned long micros(void)
{
    return (unsigned long)arduinoPlatformMicros();
}

void delay(unsigned long milliseconds)
{
    if (milliseconds == 0UL) {
        return;
    }

    uint64_t ticks = (((uint64_t)milliseconds * configTICK_RATE_HZ) + 999ULL) / 1000ULL;
    if (ticks == 0ULL) {
        ticks = 1ULL;
    }

    const uint64_t maxChunk = (uint64_t)portMAX_DELAY - 1ULL;
    while (ticks > 0ULL) {
        const TickType_t chunk = (TickType_t)(ticks > maxChunk ? maxChunk : ticks);
        vTaskDelay(chunk);
        ticks -= chunk;
    }
}

void delayMicroseconds(unsigned int microseconds)
{
    arduinoPlatformDelayMicroseconds((uint32_t)microseconds);
}

void yield(void)
{
    taskYIELD();
}

void noInterrupts(void)
{
    __disable_irq();
}

void interrupts(void)
{
    __enable_irq();
}

}
