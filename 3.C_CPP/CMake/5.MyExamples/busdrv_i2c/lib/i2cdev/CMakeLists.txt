add_library(i2cdev
    i2c.c
    swi2c.c
    hwi2c.c
)

target_include_directories(i2cdev PUBLIC inc)

target_link_libraries(tm1638 PRIVATE
    pico_stdlib
    hardware_gpio
    hardware_i2c
)
