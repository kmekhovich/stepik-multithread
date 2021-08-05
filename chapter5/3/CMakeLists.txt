cmake_minimum_required(VERSION 3.19)
project(test)


add_executable(server server.c)
set_property(TARGET server PROPERTY C_STANDARD 99)
