#include "Arduino.h"
#include "wrapper_os.h"
#include "gd32vw55x_platform.h"
#include "wifi_init.h"
#include "ble_init.h"
#include "user_setting.h"
#include "util.h"

/**
 * @brief Initialize essential applications
 * @note This is a minimal version of official application_init()
 *       Only keeps Wi-Fi/BLE required for Arduino.
 *       Optional features like FATFS/Matter/Azure are disabled
 *       to save RAM for Arduino sketches.
 */
static void application_init(void)
{
    util_init();            // Common utilities, required
    user_setting_init();    // Load Wi-Fi settings from NVDS, required

#ifdef CFG_BLE_SUPPORT
    ble_init(false);        // Init BLE stack, false = not auto advertising
#endif

#ifdef CFG_WLAN_SUPPORT
    wifi_init();            // Init Wi-Fi driver and lwIP
#endif

    // Optional features - disabled for minimal Arduino core
    // Uncomment if you need them for advanced libraries:

    // #ifdef CONFIG_FATFS_SUPPORT
    // fatfs_mk_mount(NULL);   // Mount FATFS on Flash, for SD/LittleFS library
    // #endif

    // #ifdef CFG_MATTER
    // MatterInit();           // Init Matter over Wi-Fi, for ArduinoMatter library
    // #endif

    // #ifdef CONFIG_AZURE_IOT_SUPPORT
    // azure_task_start();     // Start Azure IoT task, for Azure library
    // #endif
}

/**
 * @brief Arduino loop task - runs setup() and loop() in FreeRTOS
 * @note This task is created after FreeRTOS scheduler is initialized,
 *       similar to ESP32-Arduino's loopTask and BL602's arduino_task.
 *       Stack size 4096, priority 5. Must call sys_msleep to yield.
 */
static void arduino_loop_task(void *param)
{
    (void)param;

    init();         // Init Arduino core (SysTimer for millis/micros)
    initVariant();  // Init variant specific pins

    setup();        // User setup()

    for (;;) {
        loop();             // User loop()
        sys_msleep(1);      // Yield to Wi-Fi/BLE tasks, critical
    }
}

/**
 * @brief Main entry - C linkage required
 * @note Must be extern "C" because startup code (startup.S) is in C/ASM
 *       and expects symbol "main", not mangled C++ name "_Z4mainv".
 *       FreeRTOS is also a C library, task function pointers must be C linkage.
 */
extern "C" int main(void)
{
    sys_os_init();      // Init FreeRTOS heap and scheduler
    platform_init();    // Init clock, GPIO, UART

    application_init();

    // Create Arduino task, same as BL602-Arduino-Core's arduino_task
    // Stack: 4096 bytes, Priority: 5
    if (sys_task_create_dynamic((const uint8_t *)"arduino",
            4096, OS_TASK_PRIORITY(5), arduino_loop_task, NULL) == NULL) {
        // Task creation failed
    }

    sys_os_start();     // Start scheduler, never returns

    return 0;
}