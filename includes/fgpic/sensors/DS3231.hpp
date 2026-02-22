#ifndef FGPIC_SENSORS_DS3231_HPP
#define FGPIC_SENSORS_DS3231_HPP

#include "fgpic/i2c.hpp"
#include <cstdint>



class DS3231: public I2CDevice
{
protected:
    struct DS3231_REG {
        static constexpr uint8_t ADDR_I2C = 0x68; // DS3231 address

        static constexpr uint8_t ADDR_START = 0x00;

        static constexpr uint8_t ADDR_SECONDS = 0x00;
        static constexpr uint8_t ADDR_MINUTES = 0x01;
        static constexpr uint8_t ADDR_HOURS   = 0x02;
        static constexpr uint8_t ADDR_DAY     = 0x03;
        static constexpr uint8_t ADDR_DATE    = 0x04;
        static constexpr uint8_t ADDR_MONTH   = 0x05;
        static constexpr uint8_t ADDR_YEAR    = 0x06;

        static constexpr uint8_t ADDR_ALARM1_SECONDS = 0x07;
        static constexpr uint8_t ADDR_ALARM1_MINUTES = 0x08;
        static constexpr uint8_t ADDR_ALARM1_HOURS   = 0x09;
        static constexpr uint8_t ADDR_ALARM1_DAYDATE = 0x0A;

        static constexpr uint8_t ADDR_ALARM2_MINUTES = 0x0B;
        static constexpr uint8_t ADDR_ALARM2_HOURS   = 0x0C;
        static constexpr uint8_t ADDR_ALARM2_DAYDATE = 0x0D;

        static constexpr uint8_t ADDR_CONTROL         = 0x0E;
        static constexpr uint8_t ADDR_CONTROL_STATUS  = 0x0F;
        static constexpr uint8_t ADDR_AGING_OFFSET    = 0x10;
        static constexpr uint8_t ADDR_MSB_TEMPERATURE = 0x11;
        static constexpr uint8_t ADDR_LSB_TEMPERATURE = 0x12;
    };
    static constexpr uint DS3231_DEFAULT_TIMEOUT = 1000;
public:

    struct Time {
        uint8_t		second;
        uint8_t		minute;
        uint8_t		hour;
        uint8_t		day;
        uint8_t		date;
        uint8_t		month;
        uint16_t	year;

        Time():
        second(0),
        minute(0),
        hour(0),
        day(0),
        date(0),
        month(0),
        year(0)
        {};
    };


    DS3231(I2CInterface* i2c_bus);

    int read_time(Time& time);

private:

    uint8_t convert_seconds();
    uint8_t convert_minutes();
    uint8_t convert_hours();
    uint8_t convert_day();
    uint8_t convert_date();
    uint8_t convert_month();
    uint16_t convert_year();

    uint8_t convert_60(uint8_t value);

    uint8_t m_buf[7];

};

#endif // FGPIC_SENSORS_DS3231_HPP
