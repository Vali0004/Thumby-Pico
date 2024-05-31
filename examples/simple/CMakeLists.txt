set(PROJECT simple)
# Set as executable
add_executable(${PROJECT}
        main.c
)
# Pull in our includes
target_include_directories(${PROJECT} PUBLIC 
        ../../thumby
)
# Pull in our libs
target_link_libraries(${PROJECT}
        hardware_pwm
        hardware_spi
        pico_stdlib)
# Create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT})
# Enable uart
pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 1)