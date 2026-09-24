#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <stdint.h>
#include "gd32vw55x.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin description for mapping Arduino pin number to GPIO port/pin
typedef struct {
    uint32_t port;      // GPIOA, GPIOB, etc
    uint32_t bit;       // GPIO_PIN_0..15
    uint32_t rcu;       // RCU_GPIOx
} PinMap;

extern const PinMap g_pinMap[];
extern const uint8_t g_pinMapSize;

uint32_t getGpioPort(uint8_t pin);
uint32_t getGpioPin(uint8_t pin);
uint32_t getRcuPeriph(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
