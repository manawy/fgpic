#ifndef FGPIC_SENSORS_SHT35_HPP
#define FGPIC_SENSORS_SHT35_HPP

#include "fgpic/i2c.hpp"
#include <cstdint>

class SHT35 : public I2CDevice
{
protected:
    struct SHT35_REG {
        static constexpr uint8_t I2C_ADDR_DEFAULT   = 0x44;
        static constexpr uint8_t I2C_ADDR_ALTERNATE = 0x45;

        static constexpr uint8_t CMD_SOFTRESET[2]      = {0x30, 0xA2}; // Software reset of the sensor
        static constexpr uint8_t CMD_BREAK[2]          = {0x30, 0x93}; // Stop a continuous measurement
        static constexpr uint8_t CMD_HEATER_ENABLE[2]  = {0x30, 0x6D}; // Enable the heater
        static constexpr uint8_t CMD_HEATER_DISABLE[2] = {0x30, 0x66}; // Disable the heater

        static constexpr uint8_t CMD_STATUS[2]         = {0xF3, 0x2D};
        static constexpr uint8_t CMD_STATUS_CLEAR[2]   = {0x30, 0x41};

        static constexpr uint8_t CMD_SINGLE_HIGH_STRETCH[2]   = {0x2C, 0x06};
        static constexpr uint8_t CMD_SINGLE_MEDIUM_STRETCH[2] = {0x2C, 0x0D};
        static constexpr uint8_t CMD_SINGLE_LOW_STRETCH[2]    = {0x2C, 0x10};

        static constexpr uint8_t CMD_SINGLE_HIGH[2]   = {0x24, 0x00};
        static constexpr uint8_t CMD_SINGLE_MEDIUM[2] = {0x24, 0x0B};
        static constexpr uint8_t CMD_SINGLE_LOW[2]    = {0x24, 0x16};

        static constexpr uint8_t CMD_REPEAT_05MPS_HIGH[2]   = {0x20, 0x32};
        static constexpr uint8_t CMD_REPEAT_05MPS_MEDIUM[2] = {0x20, 0x24};
        static constexpr uint8_t CMD_REPEAT_05MPS_LOW[2]    = {0x20, 0x2F};

        static constexpr uint8_t CMD_REPEAT_1MPS_HIGH[2]    = {0x21, 0x30};
        static constexpr uint8_t CMD_REPEAT_1MPS_MEDIUM[2]  = {0x21, 0x26};
        static constexpr uint8_t CMD_REPEAT_1MPS_LOW[2]     = {0x21, 0x2D};

        static constexpr uint8_t CMD_REPEAT_2MPS_HIGH[2]    = {0x22, 0x36};
        static constexpr uint8_t CMD_REPEAT_2MPS_MEDIUM[2]  = {0x22, 0x20};
        static constexpr uint8_t CMD_REPEAT_2MPS_LOW[2]     = {0x22, 0x2B};

        static constexpr uint8_t CMD_REPEAT_4MPS_HIGH[2]    = {0x23, 0x34};
        static constexpr uint8_t CMD_REPEAT_4MPS_MEDIUM[2]  = {0x23, 0x22};
        static constexpr uint8_t CMD_REPEAT_4MPS_LOW[2]     = {0x23, 0x29};

        static constexpr uint8_t CMD_REPEAT_10MPS_HIGH[2]   = {0x27, 0x37};
        static constexpr uint8_t CMD_REPEAT_10MPS_MEDIUM[2] = {0x27, 0x21};
        static constexpr uint8_t CMD_REPEAT_10MPS_LOW[2]    = {0x27, 0x2A};

        static constexpr uint8_t CMD_FETCH_COMMAND[2]       = {0xE0, 0x00};

        static constexpr uint8_t STATUS_MSB_ALERT_MASK = 0x80; // Alert status
        static constexpr uint8_t STATUS_MSB_ALERT_OFF  = 0x00; // No pending alerts
        static constexpr uint8_t STATUS_MSB_ALERT_ON   = 0x80; // At least one pending alert

        static constexpr uint8_t STATUS_MSB_HEATER_MASK = 0x20; // Heater mask
        static constexpr uint8_t STATUS_MSB_HEATER_ON   = 0x00; // Heater is OFF
        static constexpr uint8_t STATUS_MSB_HEATER_OFF  = 0x20; // Heater is ON

        static constexpr uint8_t STATUS_MSB_RH_ALERT_MASK = 0x08; // RH tracking alert mask
        static constexpr uint8_t STATUS_MSB_RH_ALERT_ON   = 0x00; // no alert
        static constexpr uint8_t STATUS_MSB_RH_ALERT_OFF  = 0x08; // alert

        static constexpr uint8_t STATUS_MSB_T_ALERT_MASK = 0x04; // T tracking alert mask
        static constexpr uint8_t STATUS_MSB_T_ALERT_ON   = 0x00; // no alert
        static constexpr uint8_t STATUS_MSB_T_ALERT_OFF  = 0x04; // alert

        static constexpr uint8_t STATUS_LSB_SYSTEM_RESET_MASK = 0x10; // Detection system reset
        static constexpr uint8_t STATUS_LSB_NO_SYSTEM_RESET   = 0x00; // No reset detected
        static constexpr uint8_t STATUS_LSB_SYSTEM_RESET      = 0x10; // Reset detected

        static constexpr uint8_t STATUS_LSB_CMD_STATUS_MASK = 0x02; // Last command status
        static constexpr uint8_t STATUS_LSB_CMD_SUCCESS     = 0x00; // Last command success
        static constexpr uint8_t STATUS_LSB_CMD_ERROR       = 0x02; // Last command not processed

        static constexpr uint8_t STATUS_LSB_CHECKSUM_STATUS_MASK = 0x01; // Data checksum status
        static constexpr uint8_t STATUS_LSB_CHECKSUM_SUCCESS     = 0x00; // Last checksum was correct
        static constexpr uint8_t STATUS_LSB_CHECKSUM_ERROR       = 0x01; // Last checksum failed


    };
    static constexpr uint SHT35_DEFAULT_TIMEOUT = 1000;



public:
    enum SHT35Addr:uint8_t {
        SHT35Addr_default   = SHT35_REG::I2C_ADDR_DEFAULT,
        SHT35Addr_alternate = SHT35_REG::I2C_ADDR_ALTERNATE
    };

    enum SHT35Repeatability: uint8_t  {
        SHT35Repeatability_low    = 0,
        SHT35Repeatability_medium = 1,
        SHT35Repeatability_high   = 2
    };

    enum SHT35MPS: uint8_t  {
        SHT35MPS_05  =  5,
        SHT35MPS_1   =  1,
        SHT35MPS_2   =  2,
        SHT35MPS_4   =  4,
        SHT35MPS_10  = 10
    };

    struct SHT35Config {
      bool single_measurement = true;
      bool clock_stretching = false;
      SHT35Repeatability repeatability = SHT35Repeatability_high;
      SHT35MPS mps = SHT35MPS_1;
    };

    SHT35(I2CInterface* i2cbus, uint8_t addr);

    int read_once();
    int start_repeat_measurement();
    int stop_repeat_measurement();
    int fetch_data();
    float temperature() {return m_temperature;}
    float relative_humidity() {return m_rh;}
    int disable_heater();
    int enable_heater();

    void set_mps(SHT35MPS mps) {m_config.mps = mps;}
    void set_repeatability(SHT35Repeatability repeatability) {m_config.repeatability = repeatability;}

private:


    // Set the measurement command in the byffer according the config
    void set_measurement_command();
    void convert_data();


    SHT35Config m_config;

    float m_temperature;
    float m_rh;

    uint8_t m_buf[7];
    uint8_t m_cmd_buf[2];

};

#endif // FGPIC_SENSORS_SHT35_HPP
