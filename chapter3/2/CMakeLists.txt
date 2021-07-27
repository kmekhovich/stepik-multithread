cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server.cpp)
add_executable(client client.cpp)