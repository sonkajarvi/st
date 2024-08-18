cmake_minimum_required(VERSION 3.20)

project(rendertest LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(rendertest
    engine.c
    main.c
    window.c
    window_x11.c
)

target_link_libraries(rendertest PRIVATE
    X11 GL
)

target_compile_options(rendertest PRIVATE
    -g
    -O2
    -Wall
    -Wextra
    -Werror
    -pedantic
    # -Wconversion
    -Wstrict-prototypes
    -Wno-gnu-zero-variadic-macro-agruments
)
