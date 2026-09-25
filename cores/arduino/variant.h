#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <stdint.h>
#include <stdbool.h>
#include "gd32vw55x.h"
#include "variant_config.h"

#ifndef ARDUINO_VARIANT_HAS_WIFI
#define ARDUINO_VARIANT_HAS_WIFI 0
#endif

#ifndef ARDUINO_VARIANT_HAS_BLE
#define ARDUINO_VARIANT_HAS_BLE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Pin description for mapping Arduino pin number to GPIO port/pin
typedef struct {
    uint32_t port;      // GPIOA, GPIOB, etc
    uint32_t bit;       // GPIO_PIN_0..15
    uint32_t rcu;       // RCU_GPIOx
} PinMap;

typedef struct {
    uint32_t timer;
    uint32_t timerRcu;
    uint32_t period;
    uint16_t channel;
    uint16_t prescaler;
    uint8_t alternateFunction;
} PwmPinMap;

extern const PinMap g_pinMap[];
extern const uint8_t g_pinMapSize;

uint32_t getGpioPort(uint8_t pin);
uint32_t getGpioPin(uint8_t pin);
uint32_t getRcuPeriph(uint8_t pin);
int getAdcChannel(uint8_t pin);
bool getPwmPinMap(uint8_t pin, PwmPinMap *map);

#ifdef __cplusplus
}
#endif

#endif
