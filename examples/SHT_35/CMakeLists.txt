add_executable(sht_35
    sht_35.cpp
    )

pico_enable_stdio_usb(sht_35 1)
pico_enable_stdio_uart(sht_35 0)

# pull in common dependencies
target_link_libraries(sht_35 pico_stdlib hardware_i2c pico_binary_info fgpic_sht35)

pico_add_extra_outputs(sht_35)
