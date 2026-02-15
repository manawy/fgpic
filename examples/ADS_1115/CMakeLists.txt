

add_executable(ads_1115
    ads_1115.cpp
    )

pico_enable_stdio_usb(ads_1115 1)
pico_enable_stdio_uart(ads_1115 0)

# pull in common dependencies
target_link_libraries(ads_1115 pico_stdlib hardware_i2c pico_binary_info fgpic_ads1115)

pico_add_extra_outputs(ads_1115)
