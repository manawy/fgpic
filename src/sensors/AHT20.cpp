#include "fgpic/sensors/AHT20.hpp"
#include <cstring>

AHT20::AHT20(I2CInterface* bus, AHT20IC2Addr address):
    I2CDevice(bus, address)
{
    memset(m_bytes, 0, 7);
}

int AHT20::measureNoCRC() {

    uint8_t cmd[3] = {AHT20CMD_TRIGGER, 0x33, 0x00};
    if (write_blocking(cmd, 3, false) != 3) {
        return AHT20ReturnCode_ERROR_CONNECT;
    }

    sleep_ms(80);

    while (isBusy()) {
        sleep_ms(2);
    }

    if (read_blocking(m_bytes, 6, false) != 6) {
        return AHT20ReturnCode_ERROR_CONNECT;
    }

    convertAndSet();
    return AHT20ReturnCode_OK;
}

void AHT20::convertAndSet(void) {
    uint32_t raw = m_bytes[1];
    raw <<= 8;
    raw += m_bytes[2];
    raw <<= 4;
    raw += (m_bytes[3] >> 4);
    m_humidity = raw * 9.5367431640625e-5; // ==> / 1048576.0 * 100%;

    raw = (m_bytes[3] & 0x0F);
    raw <<= 8;
    raw += m_bytes[4];
    raw <<= 8;
    raw += m_bytes[5];
    m_temperature = raw * 1.9073486328125e-4 - 50; //  ==> / 1048576.0 * 200 - 50;
}

float AHT20::getTemperature() {
    return m_temperature;
}

float AHT20::getHumidity() {
    return m_humidity;
}

uint8_t AHT20::init(bool no_sleep) {
    if (!no_sleep) {
        sleep_ms(100);
    }


    uint8_t cmd[3];

    // ask for a reset, if cannot write command, return
    cmd[0] = AHT20CMD_SOFTRESET;
    if (!write_blocking(cmd, 1, false))  {
        return AHT20ReturnCode_ERROR_CONNECT;
    }
    sleep_ms(20);

    while (getStatus() & AHT20StatusCode_BUSY) {
        sleep_ms(10);
    } // wait until free

    // calibration command
    cmd[0] = AHT20CMD_CALIBRATE;
    cmd[1] = 0x08;
    cmd[2] = 0x00;
    write_blocking(cmd, 3, false); // may not 'succeed' on newer AHT20s


    while (isBusy()) {
        sleep_ms(10);
    }
    if (!isCalibrated()) {
        return AHT20ReturnCode_ERROR_CALIBRATION;
    }

    return AHT20ReturnCode_OK;
}

bool AHT20::isBusy(void) {
    uint8_t status;
    if (!read_blocking(&status, 1, false) == 1) {
        return false;
    }
    return ((status & AHT20StatusCode_BUSY) == AHT20StatusCode_BUSY);
}

bool AHT20::isCalibrated(void) {
    uint8_t status;
    if (!read_blocking(&status, 1, false) == 1) {
        return false;
    }
    return ((status & AHT20StatusCode_CALIBRATED) == AHT20StatusCode_CALIBRATED);
}

uint8_t AHT20::getStatus(void) {
    uint8_t ret;
    if (!read_blocking(&ret, 1, false)) {
        return 0xFF; // if nothing read
    }
    return ret;
}
