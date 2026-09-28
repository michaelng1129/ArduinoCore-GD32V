#include "HardwareSerial.h"
#include "gd32vw55x.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_usart.h"
#include "uart.h"

// Arduino Serial on the GD32VW553K-START is UART2 on PA6 (TX) / PA7 (RX),
// wired to the on-board GD-Link USB serial (AN154 Figure 1-2).
// UART2 pinmux (GD32VW553-MINI Datasheet Table 4-1, MSDK uart_config.h):
//   PA6 = UART2_TX, alternate function AF10
//   PA7 = UART2_RX, alternate function AF8
//
// NOTE: the vendor SDK log output (LOG_UART) also uses UART2 on START, so
// SDK log messages and Serial output share the same USB serial port.
// Serial.begin() takes over the peripheral configuration; the SDK log
// keeps working on top of it (it polls the TX register directly).
//
// RX uses the SDK interrupt framework: the SDK already defines
// UART2_IRQHandler (gd32vw55x_it.c), which dispatches to a callback
// registered with uart_irq_callback_register(). We must NOT define our own
// UART2_IRQHandler - that would be a duplicate symbol at link time.

#define ARDUINO_SERIAL_USART UART2
#define ARDUINO_SERIAL_TX_PIN GPIO_PIN_6
#define ARDUINO_SERIAL_TX_AF GPIO_AF_10
#define ARDUINO_SERIAL_RX_PIN GPIO_PIN_7
#define ARDUINO_SERIAL_RX_AF GPIO_AF_8

// RX ring buffer. Written by the SDK-dispatched RX callback (in interrupt
// context), read by the Arduino task. Head is advanced only by the ISR,
// tail only by the task, so no lock is needed; the indices are volatile
// so the compiler does not cache them.
#define SERIAL_RX_BUFFER_SIZE 256U
#define SERIAL_RX_BUFFER_MASK (SERIAL_RX_BUFFER_SIZE - 1U)

static volatile uint8_t g_rxBuffer[SERIAL_RX_BUFFER_SIZE];
static volatile uint16_t g_rxHead = 0U;
static volatile uint16_t g_rxTail = 0U;

// Called by the SDK's UART2_IRQHandler via uart_irq_hdl().
static void arduinoSerialRxCallback(uint32_t uart_port)
{
    if (uart_port != ARDUINO_SERIAL_USART) {
        return;
    }

    // Clear a stale overrun flag first; otherwise the interrupt stays
    // asserted and no new data arrives.
    if (usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_ORERR)) {
        usart_flag_clear(ARDUINO_SERIAL_USART, USART_FLAG_ORERR);
    }

    // Drain the RX register into the ring buffer. Bytes are dropped only
    // when the buffer is completely full.
    while (usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_RBNE)) {
        const uint8_t c = (uint8_t)(usart_data_receive(ARDUINO_SERIAL_USART) & 0xFFU);
        const uint16_t next = (uint16_t)((g_rxHead + 1U) & SERIAL_RX_BUFFER_MASK);
        if (next != g_rxTail) {
            g_rxBuffer[g_rxHead] = c;
            g_rxHead = next;
        }
    }
}

HardwareSerial Serial;
HardwareSerial::HardwareSerial() {}

void HardwareSerial::begin(unsigned long baud) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_UART2);

    // TX: PA6, AF10, push-pull. RX: PA7, AF8, pull-up.
    gpio_af_set(GPIOA, ARDUINO_SERIAL_TX_AF, ARDUINO_SERIAL_TX_PIN);
    gpio_af_set(GPIOA, ARDUINO_SERIAL_RX_AF, ARDUINO_SERIAL_RX_PIN);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
                  ARDUINO_SERIAL_TX_PIN | ARDUINO_SERIAL_RX_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ,
                            ARDUINO_SERIAL_TX_PIN | ARDUINO_SERIAL_RX_PIN);

    usart_deinit(ARDUINO_SERIAL_USART);
    usart_baudrate_set(ARDUINO_SERIAL_USART, (uint32_t)baud);
    usart_word_length_set(ARDUINO_SERIAL_USART, USART_WL_8BIT);
    usart_stop_bit_set(ARDUINO_SERIAL_USART, USART_STB_1BIT);
    usart_parity_config(ARDUINO_SERIAL_USART, USART_PM_NONE);
    usart_transmit_config(ARDUINO_SERIAL_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(ARDUINO_SERIAL_USART, USART_RECEIVE_ENABLE);
    usart_interrupt_enable(ARDUINO_SERIAL_USART, USART_INT_RBNE);
    usart_enable(ARDUINO_SERIAL_USART);

    // Fresh buffer, hook the SDK RX dispatcher, then route the UART2
    // interrupt to the CPU. Level 1 is above the ECLIC threshold (0)
    // set up by the SDK.
    g_rxHead = 0U;
    g_rxTail = 0U;
    uart_irq_callback_register(ARDUINO_SERIAL_USART, arduinoSerialRxCallback);
    ECLIC_SetLevelIRQ(UART2_IRQn, 1);
    ECLIC_EnableIRQ(UART2_IRQn);
}

void HardwareSerial::end() {
    ECLIC_DisableIRQ(UART2_IRQn);
    uart_irq_callback_unregister(ARDUINO_SERIAL_USART);
    usart_interrupt_disable(ARDUINO_SERIAL_USART, USART_INT_RBNE);
    usart_disable(ARDUINO_SERIAL_USART);
    g_rxHead = 0U;
    g_rxTail = 0U;
}

int HardwareSerial::available() {
    const uint16_t head = g_rxHead;
    const uint16_t tail = g_rxTail;
    return (int)((head - tail) & SERIAL_RX_BUFFER_MASK);
}

int HardwareSerial::read() {
    if (g_rxHead == g_rxTail) {
        return -1;
    }
    const uint8_t c = g_rxBuffer[g_rxTail];
    g_rxTail = (uint16_t)((g_rxTail + 1U) & SERIAL_RX_BUFFER_MASK);
    return (int)c;
}

int HardwareSerial::peek() {
    if (g_rxHead == g_rxTail) {
        return -1;
    }
    return (int)g_rxBuffer[g_rxTail];
}

void HardwareSerial::flush() {
    // Wait until the last byte has fully shifted out.
    while (!usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_TC)) {}
}

size_t HardwareSerial::write(uint8_t value) {
    while (!usart_flag_get(ARDUINO_SERIAL_USART, USART_FLAG_TBE)) {}
    usart_data_transmit(ARDUINO_SERIAL_USART, value);
    return 1;
}
