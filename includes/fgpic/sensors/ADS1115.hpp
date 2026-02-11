#ifndef FGPIC_SENSORS_ADS1115_HPP
#define FGPIC_SENSORS_ADS1115_HPP

#include "fgpic/i2c.hpp"


class ADS1115: public I2CDevice
{
public:
  ADS1115::ADS1115(I2CInterface* bus, int address);

private:

};

#endif // FGPIC_SENSORS_ADS1115_HPP
