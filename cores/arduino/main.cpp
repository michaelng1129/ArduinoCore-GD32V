#include "Arduino.h"
#include "ArduinoPlatform.h"

static void arduino_loop_task(void *context)
{
    (void)context;

    init();
    initVariant();
    setup();

    for (;;) {
        loop();
        // Cooperative yield: let equal-priority tasks run. Higher-priority
        // SDK tasks (WiFi/BLE) preempt this task on their own, so no fixed
        // sleep is needed here; sketches that want to sleep call delay().
        yield();
    }
}

extern "C" int main(void)
{
    arduinoPlatformOsInit();
    arduinoPlatformHardwareInit();
    arduinoPlatformServicesInit();

    // Arduino task priority 0 -> OS_TASK_PRIORITY(0) = 16, deliberately below
    // the vendor SDK tasks (WiFi mgmt 18, BLE stack 18, BLE app 17) so the
    // radio stacks always preempt the sketch instead of starving behind it.
    if (!arduinoPlatformCreateTask("arduino", 4096, 0,
            arduino_loop_task, NULL)) {
        for (;;) {
        }
    }

    arduinoPlatformStartScheduler();

    for (;;) {
    }
}
