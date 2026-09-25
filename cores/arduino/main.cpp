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
        arduinoPlatformSleep(1);
    }
}

extern "C" int main(void)
{
    arduinoPlatformOsInit();
    arduinoPlatformHardwareInit();
    arduinoPlatformServicesInit();

    if (!arduinoPlatformCreateTask("arduino", 4096, 5,
            arduino_loop_task, NULL)) {
        for (;;) {
        }
    }

    arduinoPlatformStartScheduler();

    for (;;) {
    }
}
