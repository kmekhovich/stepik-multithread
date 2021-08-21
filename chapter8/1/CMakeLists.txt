cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_FLAGS "-O0 -fopenmp")

add_executable(server server.cpp)
