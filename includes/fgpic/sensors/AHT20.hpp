#ifndef FGPIC_SENSORS_AHT20_HPP
#define FGPIC_SENSORS_AHT20_HPP

#include "fgpic/i2c.hpp"


class AHT20 : public I2CDevice {
public:
    enum AHT20CMD :uint8_t
    {
        AHT20CMD_CALIBRATE =  0xE1,   // Calibration command
        AHT20CMD_TRIGGER   =  0xAC,   // Trigger reading command
        AHT20CMD_SOFTRESET =  0xBA,   // Soft reset command
        AHT20CMD_STATUS    =  0x71   // Ask status
    };

     enum AHT20IC2Addr :uint8_t
    {
      AHT20IC2Addr_Default   = 0x38, // Default address
      AHT20IC2Addr_Alternate = 0x39  // Alternate address, not always available
    };

    enum AHT20StatusCode : uint8_t
    {
        AHT20StatusCode_BUSY       = 0x80, // Status bit for busy status
        AHT20StatusCode_CALIBRATED = 0x08 // Status bit for calibrated sensor
    };


    enum AHT20ReturnCode : int8_t
    {
      AHT20ReturnCode_OK                =   0,    // all good
      AHT20ReturnCode_ERROR_CONNECT     = -10,  // bad connection
      AHT20ReturnCode_ERROR_BUSY        = -20,  // device busy
      AHT20ReturnCode_ERROR_CALIBRATION = -30  // device not calibrated
    };


    AHT20(I2CInterface* bus, AHT20IC2Addr address=AHT20IC2Addr_Default);

    int measureNoCRC(void);

    uint8_t init(bool no_sleep=false);
    uint8_t getStatus(void);
    bool isBusy(void);
    bool isCalibrated(void);

    float getTemperature();
    float getHumidity();


private:

    void convertAndSet(void);

    uint8_t m_bytes[7];
    uint32_t m_lastRead;

    float m_humidity;
    float m_temperature;
};

#endif // FGPIC_SENSORS_AHT20_HPP
