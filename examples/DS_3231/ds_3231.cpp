#include "pico/stdlib.h"
#include <cstdint>
#include <cstdio>
#include "fgpic/sensors/DS3231.hpp"

static constexpr int LED_DELAY_MS=2000;

int main() {

    stdio_init_all();
    sleep_ms(5000);

    I2CInterface i2cbus(i2c0, 0, 1);
    i2cbus.init();

    DS3231 ds3231(&i2cbus);



     while (true) {

         Time time;
         ds3231.read_time(time);

         printf("%d:%d:%d\n",time.hour, time.minute, time.second);
         sleep_ms(LED_DELAY_MS);
     }

}
