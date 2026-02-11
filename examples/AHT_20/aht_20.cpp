 
#include "pico/stdlib.h"
#include <cstdio>
#include "fgpic/sensors/AHT20.hpp"

static const int LED_DELAY_MS = 10000;
static const uint8_t LED_PIN = 4;
static const uint8_t LED2_PIN = 5;


// Initialize the GPIO for the LED
void led_init(void) {
    gpio_init(LED_PIN);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
}


// Turn the LED on or off
void set_led(bool led_on) {
    gpio_put(LED_PIN, led_on);
    gpio_put(LED2_PIN, !led_on);
}

int main() {
    stdio_init_all();

    sleep_ms(5000);


    led_init();

    I2CInterface i2cbus(i2c0, 0, 1);
    i2cbus.init();

    AHT20 aht20(&i2cbus);
    int status = aht20.init();
    if (status != aht20.AHT20ReturnCode_OK){
        printf("Failed to initialize AHT20.\n");
        printf("Sensor return value %d\n", status);
    }


    while (true) {
        set_led(true);

        status = aht20.measureNoCRC();
        if (status != aht20.AHT20ReturnCode_OK)
        {
            printf("Measurement failed with error value %d\n", status);
            printf("Trying again after 10s...\n");
        }
        else
        {
            printf("--- Temperature: %5.2f °C", aht20.getTemperature());
            printf("--- Humidity: %5.2f \%RH\n", aht20.getHumidity());
        }

        set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
