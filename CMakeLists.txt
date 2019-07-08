cmake_minimum_required(VERSION 3.14)
project(ping-cpp)

include_directories(src/device src/message)
 
add_executable(test-device test/test-devvydev.cpp src/device/ping-time-linux.cpp src/device/ping-port-linux.cpp src/device/ping-device.cpp)
add_executable(test-device-ping360 test/test-device-ping360.cpp src/device/ping-time-linux.cpp src/device/ping-port-linux.cpp src/device/ping-device.cpp)
add_executable(test-message test/test-message.cpp)
