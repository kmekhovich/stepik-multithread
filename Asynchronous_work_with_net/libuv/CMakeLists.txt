cmake_minimum_required(VERSION 3.19)
project(test)

add_executable(server server.c)

target_link_libraries(server uv)