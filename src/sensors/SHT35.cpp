#include "fgpic/sensors/SHT35.hpp"
#include <cstdint>
#include <hardware/timer.h>
#include <iostream>

SHT35::SHT35(I2CInterface* i2cbus, uint8_t addr):
        I2CDevice(i2cbus, addr, SHT35_DEFAULT_TIMEOUT)
{}

int SHT35::start_repeat_measurement() {
    m_config.single_measurement = false;
    set_measurement_command();
    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        return ret;
    }
    return PICO_OK;
}

int SHT35::stop_repeat_measurement() {
    m_cmd_buf[0] = SHT35_REG::CMD_BREAK[0];
    m_cmd_buf[1] = SHT35_REG::CMD_BREAK[1];
    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        return ret;
    }
    return PICO_OK;
}

int SHT35::fetch_data() {
    m_cmd_buf[0] = SHT35_REG::CMD_FETCH_COMMAND[0];
    m_cmd_buf[1] = SHT35_REG::CMD_FETCH_COMMAND[1];
    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        //std::cout << "Error command !" << std::endl;
        return ret;
    }
    busy_wait_ms(1);
    ret = read_blocking(m_buf, 6, false);
    if (ret != 6) {
        //std::cout << "Error data reading !" << std::endl;
        return ret;
    }
    convert_data();
    return PICO_OK;
}

void SHT35::convert_data() {
    uint16_t temperature = (m_buf[0] << 8) | m_buf[1];
    uint16_t rh          = (m_buf[3] << 8) | m_buf[4];

    static constexpr uint16_t range = 65535;
    m_temperature = -45.0 + 175.0*temperature/range;
    m_rh = 100.0*rh/range;
}

int SHT35::read_once() {
    m_config.single_measurement = true;
    m_config.clock_stretching = true;
    set_measurement_command();

    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        // Error
        return ret;
    }
    busy_wait_ms(1); // wait 1ms betweem commands
    ret = read_blocking(m_buf, 6, true);
    if (ret != 6) {
        return ret;
    }
    convert_data();
    return PICO_OK;
}

int SHT35::disable_heater() {
    m_cmd_buf[0] = SHT35_REG::CMD_HEATER_DISABLE[0];
    m_cmd_buf[1] = SHT35_REG::CMD_HEATER_DISABLE[1];
    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        return ret;
    }
    return PICO_OK;
}

int SHT35::enable_heater() {
    m_cmd_buf[0] = SHT35_REG::CMD_HEATER_ENABLE[0];
    m_cmd_buf[1] = SHT35_REG::CMD_HEATER_ENABLE[1];
    int ret = write_timeout(m_cmd_buf, 2, false);
    if (ret != 2) {
        return ret;
    }
    return PICO_OK;
}

void SHT35::set_measurement_command() {
    if (m_config.single_measurement) { // Single measurements
        if (m_config.clock_stretching) {
            switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_HIGH_STRETCH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_HIGH_STRETCH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_MEDIUM_STRETCH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_MEDIUM_STRETCH[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_LOW_STRETCH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_LOW_STRETCH[1];
                    break;
                default:
                    break;
            }
        } else {
            switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_SINGLE_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_SINGLE_LOW[1];
                    break;
                default:
                    break;

            }
        }
    } else { // Multiple measurements
        switch (m_config.mps) {
            case SHT35MPS_05:
                switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_05MPS_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_05MPS_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_05MPS_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_05MPS_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_05MPS_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_05MPS_LOW[1];
                    break;
                default:
                    break;
                }
                break;
            case SHT35MPS_1:
                switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_1MPS_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_1MPS_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_1MPS_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_1MPS_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_1MPS_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_1MPS_LOW[1];
                    break;
                default:
                    break;
                }
                break;
            case SHT35MPS_2:
                switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_2MPS_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_2MPS_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_2MPS_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_2MPS_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_2MPS_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_2MPS_LOW[1];
                    break;
                default:
                    break;
                }
                break;
            case SHT35MPS_4:
                switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_4MPS_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_4MPS_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_4MPS_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_4MPS_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_4MPS_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_4MPS_LOW[1];
                    break;
                default:
                    break;
                }
                break;
            case SHT35MPS_10:
                switch (m_config.repeatability) {
                case SHT35Repeatability_high:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_10MPS_HIGH[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_10MPS_HIGH[1];
                    break;
                case SHT35Repeatability_medium:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_10MPS_MEDIUM[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_10MPS_MEDIUM[1];
                    break;
                case SHT35Repeatability_low:
                    m_cmd_buf[0] = SHT35_REG::CMD_REPEAT_10MPS_LOW[0];
                    m_cmd_buf[1] = SHT35_REG::CMD_REPEAT_10MPS_LOW[1];
                    break;
                default:
                    break;
                }
                break;
        } // switch mps
    } // Single or repeat measurement
}
