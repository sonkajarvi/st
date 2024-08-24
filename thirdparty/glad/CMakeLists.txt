cmake_minimum_required(VERSION 3.20)

project(glad LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(SOURCES src/glad.c)

if (UNIX AND NOT APPLE)
    set(SOURCES ${SOURCES} src/glad_glx.c)
endif()

add_library(glad STATIC ${SOURCES})

target_include_directories(glad PRIVATE
    include
)
