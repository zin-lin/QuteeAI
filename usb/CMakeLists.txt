# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(usb)

# Add Adafruit libraries to the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adafruit_Sensor)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adafruit_BNO055)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adafruit-ST7789-Library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Adafruit_MAX1704X)

