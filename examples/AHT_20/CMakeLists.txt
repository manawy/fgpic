

add_executable(aht_20
    aht_20.cpp
    )

pico_enable_stdio_usb(aht_20 1)
pico_enable_stdio_uart(aht_20 0)

# pull in common dependencies
target_link_libraries(aht_20 pico_stdlib hardware_i2c pico_binary_info fgpic_aht20)

pico_add_extra_outputs(aht_20)

