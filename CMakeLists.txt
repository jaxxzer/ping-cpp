cmake_minimum_required(VERSION 3.12)
project(ping-cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror")

include_directories(src/message)

add_executable(test-message test/test-message.cpp)
