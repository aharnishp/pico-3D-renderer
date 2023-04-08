cmake_minimum_required(VERSION 3.12)

# Pull in Pico SDK
include(pico_sdk_import.cmake)

project(engine3D C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

#init the sdk
pico_sdk_init()


add_executable(engine3D
        index.cpp
        model.c
        model.h
        )

add_subdirectory(pico-ssd1306)

# pull in common dependencies
target_link_libraries(engine3D pico_ssd1306 hardware_i2c hardware_adc pico_stdlib)

pico_enable_stdio_usb(engine3D 1)

# create map/bin/hex file
pico_add_extra_outputs(engine3D)
