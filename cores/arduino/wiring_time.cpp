#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {

void init(void) {
    // Core init, called from arduino_loop_task before setup()
    // Enable DWT for micros()
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

unsigned long millis(void) {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

unsigned long micros(void) {
    // Use DWT cycle counter + FreeRTOS tick
    uint32_t ticks = xTaskGetTickCount();
    uint32_t cycles = DWT->CYCCNT;
    // Assuming 160MHz, cycles to us
    uint32_t us_per_tick = portTICK_PERIOD_MS * 1000;
    return (unsigned long)(ticks * us_per_tick + cycles / (SystemCoreClock / 1000000));
}

void delay(unsigned long ms) {
    if (ms == 0) return;
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void delayMicroseconds(unsigned int us) {
    if (us == 0) return;
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < cycles) { __NOP(); }
}

void yield(void) {
    taskYIELD();
}

} // extern C
