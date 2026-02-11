add_library(fgpic INTERFACE)

target_sources(fgpic INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src/i2c.cpp
        )

target_include_directories(fgpic INTERFACE ${CMAKE_CURRENT_LIST_DIR}/includes)
target_link_libraries(fgpic INTERFACE pico_stdlib hardware_gpio)
