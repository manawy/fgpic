#ifndef FGPIC_SENSORS_ADS1115_HPP
#define FGPIC_SENSORS_ADS1115_HPP

#include "fgpic/i2c.hpp"
#include <cstdint>


class ADS1115: public I2CDevice
{
protected:
  struct ADS1115_REG {
    static constexpr uint8_t APOINTER_MASK          = 0x03; // Pointer to register
    static constexpr uint8_t APOINTER_CONVERSION    = 0x00; // Last Conversion register
    static constexpr uint8_t APOINTER_CONFIG        = 0x01; // Configuration register
    static constexpr uint8_t APOINTER_LOWTHRESHOLD  = 0x02; // Low threshold value register
    static constexpr uint8_t APOINTER_HIGHTHRESHOLD = 0x03; // High threshold value register

    static constexpr uint8_t CONFIG_MS_OS_MASK    = 0x80; // Operational status
    static constexpr uint8_t CONFIG_MS_OS_TRIGGER = 0x80; // W: single conversion trigger
    static constexpr uint8_t CONFIG_MS_OS_BUSY    = 0x00; // R: Busy status
    static constexpr uint8_t CONFIG_MS_OS_NOTBUSY = 0x00; // R: Not busy status

    static constexpr uint8_t CONFIG_MS_MUX_MASK = 0x70; // Multiplexer configuration
    static constexpr uint8_t CONFIG_MS_MUX_01   = 0x00; // AINp = AIN0 and AINn = AIN1
    static constexpr uint8_t CONFIG_MS_MUX_03   = 0x10; // AINp = AIN0 and AINn = AIN3
    static constexpr uint8_t CONFIG_MS_MUX_13   = 0x20; // AINp = AIN1 and AINn = AIN3
    static constexpr uint8_t CONFIG_MS_MUX_23   = 0x30; // AINp = AIN2 and AINn = AIN3
    static constexpr uint8_t CONFIG_MS_MUX_0G   = 0x40; // AINp = AIN0 and AINn = GND
    static constexpr uint8_t CONFIG_MS_MUX_1G   = 0x50; // AINp = AIN1 and AINn = GND
    static constexpr uint8_t CONFIG_MS_MUX_2G   = 0x60; // AINp = AIN2 and AINn = GND
    static constexpr uint8_t CONFIG_MS_MUX_3G   = 0x70; // AINp = AIN3 and AINn = GND


    static constexpr uint8_t CONFIG_MS_PGA_MASK      = 0x0E; // Programmable gain configuration
    static constexpr uint8_t CONFIG_MS_PGA_FSR_6_144 = 0x00; // FSR = +/- 6.144V
    static constexpr uint8_t CONFIG_MS_PGA_FSR_4_096 = 0x02; // FSR = +/- 4.096V
    static constexpr uint8_t CONFIG_MS_PGA_FSR_2_048 = 0x04; // FSR = +/- 2.048V
    static constexpr uint8_t CONFIG_MS_PGA_FSR_1_024 = 0x06; // FSR = +/- 1.024V
    static constexpr uint8_t CONFIG_MS_PGA_FSR_0_512 = 0x08; // FSR = +/- 0.512V
    static constexpr uint8_t CONFIG_MS_PGA_FSR_0_256 = 0x0A; // FSR = +/- 0.256V


    static constexpr uint8_t CONFIG_MS_MODE_MASK       = 0x01; // Operating mode configuration
    static constexpr uint8_t CONFIG_MS_MODE_CONTINUOUS = 0x00; // Continuous conversion mode
    static constexpr uint8_t CONFIG_MS_MODE_SINGLE     = 0x01; // Single shot mode

  };


public:
  // The possible i2c address
  enum ADS1115Addr : uint8_t
  {
    ADS1115Addr_GND = 0x48,
    ADS1115Addr_VDD = 0x49,
    ADS1115Addr_SDA = 0x50,
    ADS1115Addr_SCL = 0x51
  };

  // The possible data rates
  enum ADS1115DataRate :uint8_t
  {
    ADS1115DataRate_8   = 0x00,
    ADS1115DataRate_16  = 0x01,
    ADS1115DataRate_32  = 0x02,
    ADS1115DataRate_64  = 0x03,
    ADS1115DataRate_128 = 0x04,
    ADS1115DataRate_250 = 0x05,
    ADS1115DataRate_475 = 0x06,
    ADS1115DataRate_860 = 0x07,
  };

  enum ADS1115DGain :uint8_t
  {
    ADS1115DGain_TWO_THIRD = ADS1115_REG::CONFIG_MS_PGA_FSR_6_144,
    ADS1115DGain_ONE       = ADS1115_REG::CONFIG_MS_PGA_FSR_4_096,
    ADS1115DGain_TWO       = ADS1115_REG::CONFIG_MS_PGA_FSR_2_048,
    ADS1115DGain_FOUR      = ADS1115_REG::CONFIG_MS_PGA_FSR_1_024,
    ADS1115DGain_EIGHT     = ADS1115_REG::CONFIG_MS_PGA_FSR_0_512,
    ADS1115DGain_SIXTEEN   = ADS1115_REG::CONFIG_MS_PGA_FSR_0_256,
  };

  enum ADS1115Mux :uint8_t
  {
    ADS1115MUX_01 = ADS1115_REG::CONFIG_MS_MUX_01, // AINp = AIN0 and AINn = AIN1 (default)
    ADS1115MUX_03 = ADS1115_REG::CONFIG_MS_MUX_03, // AINp = AIN0 and AINn = AIN3
    ADS1115MUX_13 = ADS1115_REG::CONFIG_MS_MUX_13, // AINp = AIN1 and AINn = AIN3
    ADS1115MUX_23 = ADS1115_REG::CONFIG_MS_MUX_23, // AINp = AIN2 and AINn = AIN3
    ADS1115MUX_0G = ADS1115_REG::CONFIG_MS_MUX_0G, // AINp = AIN0 and AINn = GND
    ADS1115MUX_1G = ADS1115_REG::CONFIG_MS_MUX_1G, // AINp = AIN1 and AINn = GND
    ADS1115MUX_2G = ADS1115_REG::CONFIG_MS_MUX_2G, // AINp = AIN1 and AINn = GND
    ADS1115MUX_3G = ADS1115_REG::CONFIG_MS_MUX_3G, // AINp = AIN3 and AINn = GND
  };

  ADS1115(I2CInterface* bus, int address);

private:

};

#endif // FGPIC_SENSORS_ADS1115_HPP
