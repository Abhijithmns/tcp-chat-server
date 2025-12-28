cmake_minimum_required(VERSION 3.10)
project(Server C)

add_executable(Server server.c)

target_link_libraries(Server PRIVATE Utils)

set(CMAKE_C_STANDARD 99)


