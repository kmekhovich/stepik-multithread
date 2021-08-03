cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(server server.cpp)

target_link_libraries(server Boost::program_options pthread)
