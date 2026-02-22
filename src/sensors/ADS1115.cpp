#include "fgpic/sensors/ADS1115.hpp"
#include <cstdint>

#include <cstdio>
#include <hardware/timer.h>

ADS1115::ADS1115(I2CInterface* bus, ADS1115Addr address):
I2CDevice(bus, address, ADS1115_DEFAULT_TIMEOUT)
{}

ADS1115::ADS1115(I2CInterface* bus, ADS1115Addr address, int timeout):
I2CDevice(bus, address, timeout)
{}

bool ADS1115::send_config() {
    return write_blocking(m_config.buf, 3, false);
}

void ADS1115::set_multiplexer(ADS1115Mux mux) {
    m_config.buf[1] = (m_config.buf[1] & ~ADS1115_REG::CONFIG_MSB_MUX_MASK) | mux;
}

void ADS1115::set_gain(ADS1115Gain gain) {
    m_config.gain = gain;
    m_config.buf[1] = (m_config.buf[1] & ~ADS1115_REG::CONFIG_MSB_PGA_MASK) | gain;
}

int ADS1115::read_once() {

    int ret = write_timeout(m_config.buf, 3, false); // assume trigger mode for now
    if (ret != 3) {
        return ret;
    }

    busy_wait_ms(10);

    constexpr uint8_t bufr[1] = {ADS1115_REG::APOINTER_CONVERSION};
    ret = write_timeout(bufr, 1, true);
    if (ret != 1) {
        return ret;
    }

    uint8_t res[2];
    ret = read_timeout(res, 2, false);
    if (ret != 2) {
        return ret;
    }
    m_last_read = (res[0] << 8) | res[1];

    return PICO_OK;
}

float ADS1115::volts()
{
    float full_range;
    switch (m_config.gain)
    {
        case ADS1115DGain_TWO_THIRD:
            full_range = 6.144f;
            break;
        case ADS1115DGain_ONE:
            full_range = 4.096f;
            break;
        case ADS1115DGain_TWO:
            full_range = 2.048f;
            break;
        case ADS1115DGain_FOUR:
            full_range = 1.024f;
            break;
        case ADS1115DGain_EIGHT:
            full_range = 0.512f;
            break;
        case ADS1115DGain_SIXTEEN:
            full_range = 0.256f;
            break;
        default:
            full_range = 0.0f;
    }
    return m_last_read * (full_range / 32768);
}
