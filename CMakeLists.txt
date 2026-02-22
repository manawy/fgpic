cmake_minimum_required(VERSION 3.12)


# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)


project(fgpic C CXX ASM)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.2.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.2.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pico SDK
# --------
pico_sdk_init()

# FreeRTOS
# --------
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/libs/FreeRTOS-Kernel)
include(FreeRTOS_Kernel_import.cmake)


# One Wire library
# ----------------
add_subdirectory(libs/OneWire)


# the main library
# ----------------
add_library(fgpic INTERFACE)

target_sources(fgpic INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/i2c.cpp
        )

target_include_directories(fgpic INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic INTERFACE pico_stdlib hardware_gpio)

# AHT20 sensor
# ---------------

add_library(fgpic_aht20 INTERFACE)

target_sources(fgpic_aht20 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/AHT20.cpp
        )
target_include_directories(fgpic_aht20 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic_aht20 INTERFACE fgpic)


# ADS1115 sensor
# ---------------

add_library(fgpic_ads1115 INTERFACE)

target_sources(fgpic_ads1115 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/ADS1115.cpp
        )
target_include_directories(fgpic_ads1115 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic_ads1115 INTERFACE fgpic)


# DS3231 RTC
# ---------------

add_library(fgpic_ds3231 INTERFACE)

target_sources(fgpic_ds3231 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/DS3231.cpp
        )
target_include_directories(fgpic_ds3231 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic_ds3231 INTERFACE fgpic)

# SHT35
# ---------------

add_library(fgpic_sht35 INTERFACE)

target_sources(fgpic_sht35 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/SHT35.cpp
        )
target_include_directories(fgpic_sht35 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic_sht35 INTERFACE fgpic)




# build examples
# --------------

add_subdirectory(examples)
