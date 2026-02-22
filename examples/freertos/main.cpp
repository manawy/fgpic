
#include "fgpic/i2c.hpp"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "portmacrocommon.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "one_wire.h"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include "fgpic/sensors/ADS1115.hpp"
#include "fgpic/sensors/DS3231.hpp"
#include <iostream>
#include <pico/error.h>


static constexpr uint8_t LED_PIN = 4;
static constexpr uint8_t LED2_PIN = 5;

static constexpr uint8_t TEMP_PIN = 23;


static constexpr int ADCReadDelay = 1000;

static QueueHandle_t volt_queue = NULL;
static QueueHandle_t temp_queue = NULL;
static SemaphoreHandle_t i2c_mutex = NULL;

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


void taskADS1115( void * pvParameters )
{
     /* The parameter value is expected to be 1 a pointer to
      the i2cbus, I2CInterface*. */

    led_init();
    ADS1115 ads1115((I2CInterface *) pvParameters, ADS1115::ADS1115Addr_GND, 512);
    ads1115.set_multiplexer(ADS1115::ADS1115MUX_03);
    ads1115.set_gain(ADS1115::ADS1115DGain_TWO);
    float millivolts;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (true) {
        set_led(true);
        if (xSemaphoreTake(i2c_mutex, (TickType_t) 10) == pdTRUE) {
            int ret = ads1115.read_once();
            xSemaphoreGive(i2c_mutex);
            if (ret == PICO_OK) {
                millivolts = ads1115.millivolts();
                xQueueSend(volt_queue, &millivolts, 0);
                set_led(false);
            }
        } else {
            std::cout << "no access !" << std::endl;
        }
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ADCReadDelay));
    }
}

void taskTemp(void* pvParameters)
{
    One_wire one_wire(TEMP_PIN);
    one_wire.init();
    rom_address_t address{};

    float temperature;


    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (true) {
        one_wire.single_device_read_rom(address);
        one_wire.convert_temperature(address, true, false);
        temperature = one_wire.temperature(address);
        xQueueSend(temp_queue, &temperature, 0);
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ADCReadDelay));
    }
}

void taskTime(void* pvParameters)
{
    DS3231 ds3231((I2CInterface *) pvParameters);
    DS3231::Time time;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (true) {
        int ret = PICO_ERROR_GENERIC;;
        if (xSemaphoreTake(i2c_mutex, (TickType_t) 10) == pdTRUE) {
            ret = ds3231.read_time(time);
            xSemaphoreGive(i2c_mutex);
        }
        if (ret != PICO_OK) {
            std::cout << "Error fetching time. Error code " << ret << "." << std::endl;
        } else {
            std::cout << (int) time.year << "/" << (int) time.month << "/" << (int) time.date << " ";
            std::cout << (int) time.hour << ":" << (int) time.minute << ":"<< (int) time.second << std::endl;
        }
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}

void taskSerialComm(void * pvParameters) {
    float millivolt = NAN;
    float temperature = NAN;

    while(true) {
        auto ret = xQueueReceive(volt_queue, &millivolt, 0);
        if (ret == pdPASS) {
            std::cout << "V: " << millivolt << "mV" << std::endl;
        }
        ret = xQueueReceive(temp_queue, &temperature, 0);
        if (ret == pdPASS) {
            std::cout << "T: " <<temperature << "C" << std::endl;
        }


        vTaskDelay(10);
    }
}

int main()
{
     stdio_init_all();

     sleep_ms(2000);



     I2CInterface i2cbus(i2c0, 0, 1);
     i2cbus.init();
     i2c_mutex = xSemaphoreCreateMutex();

     volt_queue = xQueueCreate(2, sizeof(float));
     temp_queue = xQueueCreate(2, sizeof(float));
     xTaskCreate(taskADS1115, "ADS1115", 256, (void *) &i2cbus, 3, NULL);
     xTaskCreate(taskTemp, "taskTemp", 256, NULL, 3, NULL);
     xTaskCreate(taskSerialComm, "Serial_Comm", 256, NULL, 1, NULL);
     xTaskCreate(taskTime, "Time", 256, (void *) &i2cbus, 2, NULL);

     vTaskStartScheduler();

     return 0;
 }

