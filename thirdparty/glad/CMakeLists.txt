cmake_minimum_required(VERSION 3.20)

project(glad LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad PRIVATE
    include
)
