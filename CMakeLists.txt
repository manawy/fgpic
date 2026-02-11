cmake_minimum_required(VERSION 3.12)


# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)
project(fgpic C CXX ASM)



set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350-arm-s)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.2.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.2.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()
pico_sdk_init()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(fgpic INTERFACE)

target_sources(fgpic INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/i2c.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/AHT20.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/sensors/ADS1115.cpp
        )

target_include_directories(fgpic INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic INTERFACE pico_stdlib hardware_gpio)




add_subdirectory(examples)
