#include "fgpic/sensors/DS3231.hpp"
#include "fgpic/i2c.hpp"
#include <cstdint>

DS3231::DS3231(I2CInterface* i2c_bus) :
    I2CDevice(i2c_bus, DS3231_REG::ADDR_I2C, DS3231_DEFAULT_TIMEOUT)
{}




void DS3231::read_time(Time& time) {
    m_buf[0] = DS3231_REG::ADDR_START;
    write_timeout(m_buf, 1, false);
    read_timeout(m_buf, 7, false);

    time.second = convert_seconds();
    time.minute = convert_minutes();
    time.hour = convert_hours();
    time.day = convert_day();
    time.date = convert_date();
    time.month = convert_month();
    time.year = convert_year();
}

uint8_t DS3231::convert_seconds() {
    return convert_60(m_buf[0]);
}

uint8_t DS3231::convert_minutes() {
    return convert_60(m_buf[1]);
}

uint8_t DS3231::convert_hours() {
    uint8_t decoded;
    if (m_buf[2] & 128)
        decoded = (m_buf[2] & 15) + (12 * ((m_buf[2] & 32) >> 5));
    else
        decoded = (m_buf[2] & 15) + (10 * ((m_buf[2] & 48) >> 4));
    return decoded;
}

uint8_t DS3231::convert_day() {
    uint8_t decoded = (m_buf[3] & 0x7);
    return decoded;
}

uint8_t DS3231::convert_date() {
    return convert_60(m_buf[4]);
}

uint8_t DS3231::convert_month() {
    return convert_60(m_buf[5]);
}

uint16_t DS3231::convert_year() {
    uint8_t decoded = (m_buf[6] & 15) + 10 * ((m_buf[6] & (15 << 4)) >> 4);
    return decoded+2000;
}

uint8_t DS3231::convert_60(uint8_t value) {
    uint8_t decoded = value & 127;
    decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
    return decoded;
}
