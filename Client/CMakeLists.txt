cmake_minimum_required(VERSION 3.10)
project(Client C)

add_executable(Client client.c)

target_link_libraries(Client PRIVATE Utils)

set(CMAKE_C_STANDARD 99)

