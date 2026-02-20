add_executable(ds_3231
    ds_3231.cpp
    )

pico_enable_stdio_usb(ds_3231 1)
pico_enable_stdio_uart(ds_3231 0)

# pull in common dependencies
target_link_libraries(ds_3231 pico_stdlib hardware_i2c pico_binary_info fgpic_ds3231)

pico_add_extra_outputs(ds_3231)
