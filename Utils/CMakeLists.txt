cmake_minimum_required(VERSION 3.10)
project(Utils C)

add_library(Utils STATIC
    socketutils.c
)

target_include_directories(Utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CMAKE_C_STANDARD 99)

