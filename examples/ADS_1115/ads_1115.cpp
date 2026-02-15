
 #include "pico/stdlib.h"
#include <cstdint>
 #include <cstdio>
 #include "fgpic/sensors/ADS1115.hpp"

 static const int LED_DELAY_MS = 1000;
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

     ADS1115 ads1115(&i2cbus, ADS1115::ADS1115Addr_GND);
     ads1115.set_multiplexer(ADS1115::ADS1115MUX_01);
     ads1115.set_gain(ADS1115::ADS1115DGain_FOUR);


     while (true) {
         set_led(true);

         ads1115.set_multiplexer(ADS1115::ADS1115MUX_01);
         int16_t ret = ads1115.read_once();
         ads1115.set_multiplexer(ADS1115::ADS1115MUX_23);
         int16_t ret2 = ads1115.read_once();
         printf("%f - %f\n", ads1115.volts(ret), ads1115.volts(ret2));


         set_led(false);
         sleep_ms(LED_DELAY_MS);
     }
 }
