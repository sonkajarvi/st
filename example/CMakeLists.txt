cmake_minimum_required(VERSION 3.20)

project(example LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(example main.c)

add_subdirectory(.. rendertest)

target_include_directories(example PRIVATE
    ..
    ../thirdparty/cglm/include
    ../thirdparty/glad/include
)

target_link_libraries(example PRIVATE
    rendertest
)

target_compile_options(example PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -Werror
    -pedantic
    # -Wconversion
    -Wstrict-prototypes
    -Wno-gnu-zero-variadic-macro
)
