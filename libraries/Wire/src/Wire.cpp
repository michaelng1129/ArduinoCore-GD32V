// Wire.cpp - Arduino Wire (I2C master) for GD32VW55x.
//
// Polling-based master implementation on I2C0.

#include "Wire.h"
#include "gd32vw55x_i2c.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_gpio.h"

#define WIRE_I2C        I2C0
#define WIRE_RCU        RCU_I2C0
#define WIRE_GPIO_AF    GPIO_AF_4
#define WIRE_TIMEOUT_MS 100

TwoWire::TwoWire()
    : _begun(false), _frequency(100000),
      _txAddress(0), _txLength(0),
      _rxLength(0), _rxIndex(0),
      _onReceive(nullptr), _onRequest(nullptr)
{
}

static void wireConfigurePins(int sda, int scl)
{
    int pins[2] = {sda, scl};
    for (int i = 0; i < 2; i++) {
        uint8_t pin = (uint8_t)pins[i];
        if (pin >= g_pinMapSize) {
            continue;
        }
        uint32_t port = getGpioPort(pin);
        uint32_t bit = getGpioPin(pin);
        uint32_t rcu = getRcuPeriph(pin);
        if (port == 0U || bit == 0U || rcu == 0U) {
            continue;
        }
        rcu_periph_clock_enable(rcu);
        gpio_af_set(port, WIRE_GPIO_AF, bit);
        gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, bit);
        gpio_output_options_set(port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, bit);
    }
}

static void wireConfigureTiming(uint32_t frequency)
{
    // I2C timing for the GD32 "new-style" I2C peripheral:
    //   t_SCL = ((SCLH+1) + (SCLL+1)) * (PRESC+1) * t_PCLK1
    uint32_t pclk = rcu_clock_freq_get(CK_APB1);
    if (frequency == 0) {
        frequency = 100000;
    }

    uint32_t presc = 3;  // divide by 4
    uint32_t total = pclk / (frequency * (presc + 1));
    while (total > 510 && presc < 15) {
        presc++;
        total = pclk / (frequency * (presc + 1));
    }
    if (total < 4) {
        total = 4;
    }

    // ~40% high, ~60% low (meets t_HIGH/t_LOW minimums with margin).
    uint32_t sclh = (total * 4) / 10;
    uint32_t scll = total - sclh;
    sclh = (sclh > 0) ? sclh - 1 : 0;
    scll = (scll > 0) ? scll - 1 : 0;
    if (sclh > 255) sclh = 255;
    if (scll > 255) scll = 255;

    i2c_timing_config(WIRE_I2C, presc, 4, 2);
    i2c_master_clock_config(WIRE_I2C, sclh, scll);
}

void TwoWire::begin()
{
    begin(PIN_WIRE_SDA, PIN_WIRE_SCL);
}

void TwoWire::begin(uint8_t address)
{
    (void)address;
    // Slave mode not implemented; start as master.
    begin();
}

void TwoWire::begin(int sda, int scl)
{
    wireConfigurePins(sda, scl);

    rcu_periph_clock_enable(WIRE_RCU);
    i2c_deinit(WIRE_I2C);
    wireConfigureTiming(_frequency);
    i2c_enable(WIRE_I2C);

    _txLength = 0;
    _rxLength = 0;
    _rxIndex = 0;
    _begun = true;
}

void TwoWire::end()
{
    if (!_begun) {
        return;
    }
    i2c_disable(WIRE_I2C);
    rcu_periph_clock_disable(WIRE_RCU);
    _begun = false;
}

void TwoWire::setClock(uint32_t frequency)
{
    _frequency = frequency;
    if (_begun) {
        i2c_disable(WIRE_I2C);
        wireConfigureTiming(_frequency);
        i2c_enable(WIRE_I2C);
    }
}

bool TwoWire::waitFlag(uint32_t flag, bool expectSet, uint32_t timeoutMs)
{
    uint32_t start = millis();
    for (;;) {
        bool set = (i2c_flag_get(WIRE_I2C, flag) == SET);
        if (set == expectSet) {
            return true;
        }
        if ((millis() - start) >= timeoutMs) {
            return false;
        }
    }
}

void TwoWire::beginTransmission(uint8_t address)
{
    _txAddress = address;
    _txLength = 0;
}

size_t TwoWire::write(uint8_t data)
{
    if (_txLength >= WIRE_BUFFER_LENGTH) {
        return 0;
    }
    _txBuffer[_txLength++] = data;
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    size_t n = 0;
    for (size_t i = 0; i < quantity; i++) {
        if (write(data[i]) == 0) {
            break;
        }
        n++;
    }
    return n;
}

uint8_t TwoWire::endTransmission(bool stop)
{
    if (!_begun) {
        return 4;  // other error
    }

    // Clear stale NACK flag from any previous transfer.
    i2c_flag_clear(WIRE_I2C, I2C_FLAG_NACK);

    i2c_master_addressing(WIRE_I2C, (uint32_t)_txAddress << 1, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(WIRE_I2C, _txLength);
    i2c_automatic_end_disable(WIRE_I2C);
    i2c_start_on_bus(WIRE_I2C);

    for (uint8_t i = 0; i < _txLength; i++) {
        if (!waitFlag(I2C_FLAG_TBE, true, WIRE_TIMEOUT_MS)) {
            i2c_stop_on_bus(WIRE_I2C);
            return 4;
        }
        if (i2c_flag_get(WIRE_I2C, I2C_FLAG_NACK) == SET) {
            i2c_flag_clear(WIRE_I2C, I2C_FLAG_NACK);
            i2c_stop_on_bus(WIRE_I2C);
            return 2;  // NACK on address/data
        }
        i2c_data_transmit(WIRE_I2C, _txBuffer[i]);
    }

    if (!waitFlag(I2C_FLAG_TC, true, WIRE_TIMEOUT_MS)) {
        i2c_stop_on_bus(WIRE_I2C);
        return 4;
    }
    if (i2c_flag_get(WIRE_I2C, I2C_FLAG_NACK) == SET) {
        i2c_flag_clear(WIRE_I2C, I2C_FLAG_NACK);
        i2c_stop_on_bus(WIRE_I2C);
        return 3;  // NACK on data
    }

    if (stop) {
        i2c_stop_on_bus(WIRE_I2C);
    }

    _txLength = 0;
    return 0;  // success
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, bool stop)
{
    if (!_begun || quantity == 0) {
        return 0;
    }
    if (quantity > WIRE_BUFFER_LENGTH) {
        quantity = WIRE_BUFFER_LENGTH;
    }

    i2c_flag_clear(WIRE_I2C, I2C_FLAG_NACK);

    i2c_master_addressing(WIRE_I2C, (uint32_t)address << 1, I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(WIRE_I2C, quantity);
    i2c_automatic_end_disable(WIRE_I2C);
    i2c_start_on_bus(WIRE_I2C);

    _rxLength = 0;
    _rxIndex = 0;
    for (uint8_t i = 0; i < quantity; i++) {
        if (!waitFlag(I2C_FLAG_RBNE, true, WIRE_TIMEOUT_MS)) {
            break;
        }
        _rxBuffer[_rxLength++] = (uint8_t)i2c_data_receive(WIRE_I2C);
    }

    waitFlag(I2C_FLAG_TC, true, WIRE_TIMEOUT_MS);
    if (stop) {
        i2c_stop_on_bus(WIRE_I2C);
    }

    return _rxLength;
}

int TwoWire::available()
{
    return (int)(_rxLength - _rxIndex);
}

int TwoWire::read()
{
    if (_rxIndex >= _rxLength) {
        return -1;
    }
    return _rxBuffer[_rxIndex++];
}

int TwoWire::peek()
{
    if (_rxIndex >= _rxLength) {
        return -1;
    }
    return _rxBuffer[_rxIndex];
}

void TwoWire::flush()
{
    _rxIndex = _rxLength;
}

TwoWire Wire;
