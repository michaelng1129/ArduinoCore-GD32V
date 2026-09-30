// SPI.cpp - Arduino SPI library for GD32VW55x.

#include "SPI.h"
#include "gd32vw55x_spi.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_gpio.h"

// All SPI pins use AF5 (datasheet AF mapping table).
#define SPI_GPIO_AF GPIO_AF_5

SPIClass::SPIClass() : _begun(false), _settings(), _divider(0)
{
}

void SPIClass::configurePins(int sck, int miso, int mosi)
{
    int pins[3] = {sck, miso, mosi};
    for (int i = 0; i < 3; i++) {
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
        gpio_af_set(port, SPI_GPIO_AF, bit);
        gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, bit);
        gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, bit);
    }
}

void SPIClass::begin()
{
    begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
}

void SPIClass::begin(int sck, int miso, int mosi, int ss)
{
    configurePins(sck, miso, mosi);
    if (ss >= 0) {
        pinMode((uint8_t)ss, OUTPUT);
        digitalWrite((uint8_t)ss, HIGH);
    }

    rcu_periph_clock_enable(RCU_SPI);

    // Apply default settings; beginTransaction() reconfigures per device.
    applySettings(_settings);
    _begun = true;
}

void SPIClass::end()
{
    if (!_begun) {
        return;
    }
    spi_disable();
    rcu_periph_clock_disable(RCU_SPI);
    _begun = false;
}

void SPIClass::applySettings(const SPISettings &settings)
{
    uint32_t clock = settings._clock;
    if (_divider != 0) {
        // Legacy setClockDivider(): divider is a power-of-two exponent index.
        // Map Arduino DIV2/DIV4/... to a target clock.
        uint32_t base = rcu_clock_freq_get(CK_APB2);
        clock = base >> _divider;
    }

    // Choose the smallest prescaler that keeps SCK at or below target.
    uint32_t pclk = rcu_clock_freq_get(CK_APB2);
    uint32_t psc;
    if (clock == 0 || pclk / 2 <= clock) {
        psc = SPI_PSC_2;
    } else if (pclk / 4 <= clock) {
        psc = SPI_PSC_4;
    } else if (pclk / 8 <= clock) {
        psc = SPI_PSC_8;
    } else if (pclk / 16 <= clock) {
        psc = SPI_PSC_16;
    } else if (pclk / 32 <= clock) {
        psc = SPI_PSC_32;
    } else if (pclk / 64 <= clock) {
        psc = SPI_PSC_64;
    } else if (pclk / 128 <= clock) {
        psc = SPI_PSC_128;
    } else {
        psc = SPI_PSC_256;
    }

    uint32_t ckpl_ph;
    switch (settings._dataMode) {
    case SPI_MODE0:
    default:
        ckpl_ph = SPI_CK_PL_LOW_PH_1EDGE;
        break;
    case SPI_MODE1:
        ckpl_ph = SPI_CK_PL_LOW_PH_2EDGE;
        break;
    case SPI_MODE2:
        ckpl_ph = SPI_CK_PL_HIGH_PH_1EDGE;
        break;
    case SPI_MODE3:
        ckpl_ph = SPI_CK_PL_HIGH_PH_2EDGE;
        break;
    }

    spi_parameter_struct spi;
    spi_struct_para_init(&spi);
    spi.device_mode = SPI_MASTER;
    spi.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi.frame_size = SPI_FRAMESIZE_8BIT;
    spi.nss = SPI_NSS_SOFT;
    spi.endian = (settings._bitOrder == LSBFIRST) ? SPI_ENDIAN_LSB : SPI_ENDIAN_MSB;
    spi.clock_polarity_phase = ckpl_ph;
    spi.prescale = psc;

    spi_disable();
    spi_init(&spi);
    spi_enable();
}

void SPIClass::beginTransaction(SPISettings settings)
{
    _settings = settings;
    _divider = 0;
    if (_begun) {
        applySettings(settings);
    }
}

void SPIClass::endTransaction()
{
    // Single SPI peripheral shared by all devices; nothing to release.
}

uint8_t SPIClass::transfer(uint8_t data)
{
    while (spi_flag_get(SPI_FLAG_TBE) == RESET) {
    }
    spi_data_transmit(data);
    while (spi_flag_get(SPI_FLAG_RBNE) == RESET) {
    }
    return (uint8_t)spi_data_receive();
}

uint16_t SPIClass::transfer16(uint16_t data)
{
    uint16_t in;
    if (_settings._bitOrder == MSBFIRST) {
        in = (uint16_t)(transfer((uint8_t)(data >> 8)) << 8);
        in |= transfer((uint8_t)(data & 0xFF));
    } else {
        in = transfer((uint8_t)(data & 0xFF));
        in |= (uint16_t)(transfer((uint8_t)(data >> 8)) << 8);
    }
    return in;
}

void SPIClass::transfer(void *buf, size_t count)
{
    uint8_t *p = (uint8_t *)buf;
    for (size_t i = 0; i < count; i++) {
        p[i] = transfer(p[i]);
    }
}

void SPIClass::setClockDivider(uint8_t divider)
{
    // Arduino DIV constants are 0..7 mapping to /2../256.
    _divider = divider;
    if (_begun) {
        applySettings(_settings);
    }
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    _settings._dataMode = dataMode;
    if (_begun) {
        applySettings(_settings);
    }
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
    _settings._bitOrder = bitOrder;
    if (_begun) {
        applySettings(_settings);
    }
}

SPIClass SPI;
