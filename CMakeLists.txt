cmake_minimum_required(VERSION 3.12)

set(NAME Hexapod)

include(pico_sdk_import.cmake)
include(pimoroni_pico_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Define the source files
set(SOURCES
    src/main.cpp
    # src/dispatcher.hpp
)

# Define the include directories
include_directories(src src/commands)

# Add your source files
add_executable(${NAME} ${SOURCES})

# Include required libraries
# This assumes pimoroni-pico is stored alongside your project
include(libraries/servo2040/servo2040)
include(drivers/analogmux/analogmux)
include(drivers/button/button)
include(drivers/analog/analog)

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
    pico_stdlib
    servo2040
    analogmux
    analog
    button
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)