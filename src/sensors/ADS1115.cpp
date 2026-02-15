#include "fgpic/sensors/ADS1115.hpp"
#include <cstdint>

#include <cstdio>
#include <hardware/timer.h>

ADS1115::ADS1115(I2CInterface* bus, ADS1115Addr address):
I2CDevice(bus, address)
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

int16_t ADS1115::read_once() {
    write_blocking(m_config.buf, 3, false); // assume trigger mode for now

    uint8_t bufr[1] = {ADS1115_REG::APOINTER_CONVERSION};
    write_blocking(bufr, 1, false);
    busy_wait_ms(10);

    uint8_t res[2];
    read_blocking(res, 2, false);
    uint16_t ret = (res[0] << 8) | res[1];

    // printf("%d\n", ret);
    return (int16_t) ret;
}

float ADS1115::volts(int16_t conversion)
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
    return conversion * (full_range / 32768);
}
