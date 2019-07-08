cmake_minimum_required(VERSION 2.8.9)
project(ping-cpp)
 
#Bring the headers, such as Student.h into the project
include_directories(src/device src/message)
 
#Can manually add the sources using the set command as follows:
set(SOURCES src/device/ping-time-linux.cpp src/device/ping-port-linux.cpp src/device/ping-device.cpp)
 
add_executable(test-device test/test-devvydev.cpp ${SOURCES})
