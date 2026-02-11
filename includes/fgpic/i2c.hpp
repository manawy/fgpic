#ifndef FGPIC_I2C_HPP
#define FGPIC_I2C_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"


// The I2C interface used by a device
class I2CInterface {
public:
    I2CInterface(i2c_inst_t* i2cbus, int sda_pin, int scl_pin);

    void init();

    i2c_inst_t* get() const {return m_i2cbus;}

private:
    i2c_inst_t* m_i2cbus;
    const int m_sda_pin;
    const int m_scl_pin;

};

inline I2CInterface::I2CInterface(i2c_inst_t* i2cbus, int sda_pin, int scl_pin):
    m_i2cbus(i2cbus),
    m_sda_pin(sda_pin),
    m_scl_pin(scl_pin)
    { }

// A generic class for an I2C device
class I2CDevice {
public:
    I2CDevice(I2CInterface* bus, int8_t address);

    int write_blocking(const uint8_t *src, size_t len, bool nostop);

    int read_blocking(uint8_t *dst, size_t len, bool nostop);
private:
    I2CInterface* m_bus;
    const int8_t m_address;

};

inline I2CDevice::I2CDevice(I2CInterface *bus, int8_t address):
    m_bus(bus),
    m_address(address)
{}

#endif // FGPIC_I2C_HPP
