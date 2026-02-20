#include "pico/stdlib.h"
#include <cstdint>
#include <cstdio>
#include "fgpic/sensors/SHT35.hpp"

static constexpr int LED_DELAY_MS=5000;

static constexpr int LED_GREEN = 4;

static constexpr int LED_RED = 5;

int main() {

    stdio_init_all();

    gpio_init(LED_GREEN);
    gpio_init(LED_RED);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);

    sleep_ms(2000);

    I2CInterface i2cbus(i2c0, 0, 1);
    i2cbus.init();

    SHT35 sht35(&i2cbus, SHT35::SHT35Addr_default);

    sht35.set_mps(SHT35::SHT35MPS_1);
    sht35.set_repeatability(SHT35::SHT35Repeatability_high);

    sht35.disable_heater(); // just to make sure
    sleep_ms(1000);

    int ret = sht35.start_repeat_measurement();

    if (ret != PICO_OK) {
        printf("Error init SHT 35: return code %d\n", ret);
        gpio_put(LED_GREEN, false);
        gpio_put(LED_RED, true);
    } else {
        printf("SHT 35: measurement started\n");
        gpio_put(LED_GREEN, true);
        gpio_put(LED_RED, false);

    }
    //sht35.stop_repeat_measurement();
    sleep_ms(2000);

     while (true) {

        int ret = sht35.fetch_data();
        //int ret = sht35.read_once();
        if (ret != PICO_OK) {
            printf("SHT 35: Error getting data: return code %d\n", ret);
            gpio_put(LED_GREEN, false);
            gpio_put(LED_RED, true);
        } else {
            printf("%f %f \n",sht35.temperature(), sht35.relative_humidity());
        }

        sleep_ms(LED_DELAY_MS);
     }

}
