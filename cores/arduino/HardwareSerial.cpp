#include "HardwareSerial.h"
#include "gd32vw55x.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_usart.h"

#if defined(CFG_BLE_HCI_MODE)
#error "Arduino Serial uses USART0, which is reserved for BLE HCI in CFG_BLE_HCI_MODE. Select a non-conflicting Serial route before enabling Arduino."
#endif

#define ARDUINO_SERIAL_USART USART0

HardwareSerial Serial;
HardwareSerial::HardwareSerial() {}

void HardwareSerial::begin(unsigned long baud) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_0 | GPIO_PIN_1);
    usart_deinit(ARDUINO_SERIAL_USART);
    usart_baudrate_set(ARDUINO_SERIAL_USART, (uint32_t)baud);
    usart_word_length_set(ARDUINO_SERIAL_USART, USART_WL_8BIT);
    usart_stop_bit_set(ARDUINO_SERIAL_USART, USART_STB_1BIT);
    usart_parity_config(ARDUINO_SERIAL_USART, USART_PM_NONE);
    usart_transmit_config(ARDUINO_SERIAL_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(ARDUINO_SERIAL_USART, USART_RECEIVE_ENABLE);
    usart_enable(ARDUINO_SERIAL_USART);
}

void HardwareSerial::end() { usart_disable(ARDUINO_SERIAL_USART); }
int HardwareSerial::available() { return usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_RBNE) ? 1 : 0; }
int HardwareSerial::read() { return available() ? (int)(usart_data_receive(ARDUINO_SERIAL_USART) & 0xFFU) : -1; }
int HardwareSerial::peek() { return available() ? (int)(USART_RDATA(ARDUINO_SERIAL_USART) & 0xFFU) : -1; }
void HardwareSerial::flush() { while (!usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_TC)) {} }
size_t HardwareSerial::write(uint8_t value) {
    while (!usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_TBE)) {}
    usart_data_transmit(ARDUINO_SERIAL_USART, value);
    return 1;
}
