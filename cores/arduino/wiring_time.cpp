#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"
#include "core_feature_base.h"   // NMSIS: CSR read macros (RISC-V 標準)
#include "core_feature_timer.h"  // NMSIS: SysTimer struct (Nuclei 擴充)

extern "C" {

void init(void) {
    // RISC-V mcycle counter 從 reset 後就自動累加，不需要像 ARM DWT
    // 那樣手動致能 TRCENA / CYCCNTENA，這裡不需要做任何事
}

unsigned long millis(void) {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

unsigned long micros(void) {
    // 用 RISC-V 標準 CSR 讀取 mcycle（取代 ARM 的 DWT->CYCCNT）
    uint32_t cycle = __RV_CSR_READ(CSR_MCYCLE);
    return (unsigned long)(cycle / (SystemCoreClock / 1000000));
}

void delay(unsigned long ms) {
    if (ms == 0) return;
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void delayMicroseconds(unsigned int us) {
    if (us == 0) return;
    uint32_t start = __RV_CSR_READ(CSR_MCYCLE);
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    while ((__RV_CSR_READ(CSR_MCYCLE) - start) < cycles) {
        __asm__ volatile ("nop");
    }
}

void yield(void) {
    taskYIELD();
}

} // extern C
