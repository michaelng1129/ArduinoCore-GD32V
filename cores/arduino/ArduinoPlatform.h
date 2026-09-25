#ifndef ARDUINO_PLATFORM_H
#define ARDUINO_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ArduinoTaskFunction)(void *);

void arduinoPlatformOsInit(void);
void arduinoPlatformHardwareInit(void);
void arduinoPlatformServicesInit(void);
bool arduinoPlatformCreateTask(const char *name, uint32_t stackSize,
        uint32_t priority, ArduinoTaskFunction task, void *context);
void arduinoPlatformStartScheduler(void);
void arduinoPlatformSleep(uint32_t milliseconds);
uint64_t arduinoPlatformMicros(void);
void arduinoPlatformDelayMicroseconds(uint32_t microseconds);

#ifdef __cplusplus
}
#endif

#endif
