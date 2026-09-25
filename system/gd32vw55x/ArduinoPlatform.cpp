#include "Arduino.h"
#include "ArduinoPlatform.h"
#include "wrapper_os.h"
#include "gd32vw55x_platform.h"
#include "core_feature_timer.h"
#include "systime.h"

#if ARDUINO_VARIANT_HAS_WIFI || ARDUINO_VARIANT_HAS_BLE
#include "util.h"
#include "user_setting.h"
#endif

#if ARDUINO_VARIANT_HAS_WIFI
#include "wifi_init.h"
#endif

#if ARDUINO_VARIANT_HAS_BLE
#include "ble_init.h"
#endif

extern "C" void arduinoPlatformOsInit(void)
{
    sys_os_init();
}

extern "C" void arduinoPlatformHardwareInit(void)
{
    platform_init();
}

extern "C" void arduinoPlatformServicesInit(void)
{
#if ARDUINO_VARIANT_HAS_WIFI || ARDUINO_VARIANT_HAS_BLE
    util_init();
    user_setting_init();
#endif

#if ARDUINO_VARIANT_HAS_BLE
    ble_init(false);
#endif

#if ARDUINO_VARIANT_HAS_WIFI
    wifi_init();
#endif
}

extern "C" bool arduinoPlatformCreateTask(const char *name,
        uint32_t stackSize, uint32_t priority,
        ArduinoTaskFunction task, void *context)
{
    return sys_task_create_dynamic((const uint8_t *)name,
            stackSize, OS_TASK_PRIORITY(priority), task, context) != NULL;
}

extern "C" void arduinoPlatformStartScheduler(void)
{
    sys_os_start();
}

extern "C" void arduinoPlatformSleep(uint32_t milliseconds)
{
    sys_ms_sleep((int)milliseconds);
}

extern "C" uint64_t arduinoPlatformMicros(void)
{
    if (clock_us_factor == 0U) {
        return 0ULL;
    }

    return get_sys_local_time_us();
}

extern "C" void arduinoPlatformDelayMicroseconds(uint32_t microseconds)
{
    if (microseconds == 0U || clock_us_factor == 0U ||
            (SysTimer_GetControlValue() & SysTimer_MTIMECTL_TIMESTOP_Msk) != 0U) {
        return;
    }

    systick_udelay(microseconds);
}
