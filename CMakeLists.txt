cmake_minimum_required(VERSION 3.7)
project(key_value)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

add_executable(key_value main.cpp client.h client.cpp parse.h parse.cpp)
