cmake_minimum_required(VERSION 3.18)

include(pico_sdk_import.cmake)

project(pico_program C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wextra
        -O3
        )

add_executable(${PROJECT_NAME}
        main/main.cpp
)

pico_add_extra_outputs(${PROJECT_NAME})

add_library(Common INTERFACE)
target_sources(Common INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/Motors/Motors.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Servo/Servo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/pioPWM/pioPWM.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Encoder/Encoder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_font.cpp
)
target_include_directories(Common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pwm hardware_gpio hardware_i2c hardware_pio Common)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_generate_pio_header(Common ${CMAKE_CURRENT_LIST_DIR}/include/pioPWM/pwm.pio)
pico_generate_pio_header(Common ${CMAKE_CURRENT_LIST_DIR}/include/Encoder/quadrature.pio)