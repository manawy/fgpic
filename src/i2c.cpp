#include "fgpic/i2c.hpp"
#include "pico/binary_info.h"
#include <hardware/i2c.h>

//  I2CInterface
// -----------------

void I2CInterface::init() {

    i2c_init(m_i2cbus, 100 * 1000);
    gpio_set_function(m_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(m_scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(m_sda_pin);
    gpio_pull_up(m_scl_pin);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(m_sda_pin, m_scl_pin, GPIO_FUNC_I2C));

    return;
}

//   I2CDevice
// --------------

int I2CDevice::write_blocking(const uint8_t *src, size_t len, bool nostop) {
    return i2c_write_blocking(m_bus->get(), m_address, src, len, nostop);
}


int I2CDevice::write_timeout(const uint8_t *src, size_t len, bool nostop) {
    return i2c_write_timeout_us(m_bus->get(), m_address, src, len, nostop, m_timeout_us);
}



int I2CDevice::read_blocking(uint8_t *dst, size_t len, bool nostop) {
    return  i2c_read_blocking(m_bus->get(), m_address, dst, len, nostop);
}



int I2CDevice::read_timeout(uint8_t *dst, size_t len, bool nostop) {
    return  i2c_read_timeout_us(m_bus->get(), m_address, dst, len, nostop, m_timeout_us);
}
