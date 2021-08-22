cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(creator creator.cpp)
add_executable(calc calc.cpp)

target_link_libraries(calc pthread)
