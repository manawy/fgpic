add_executable(freertos_test
    main.cpp
    )

pico_enable_stdio_usb(freertos_test 1)
pico_enable_stdio_uart(freertos_test 0)

# pull in common dependencies
target_link_libraries(freertos_test pico_stdlib hardware_i2c pico_binary_info fgpic_ads1115 fgpic_ds3231 FreeRTOS-Kernel-Heap4 pico_one_wire)
target_include_directories(freertos_test  PRIVATE ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(freertos_test)
