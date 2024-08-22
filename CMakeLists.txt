cmake_minimum_required(VERSION 3.20)

project(rendertest LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(rendertest
    engine.c
    glad_loader.c
    main.c
    renderer.c
    renderer_gl.c
    util.c
    window.c
    window_x11.c
)

add_subdirectory(thirdparty/glad)

target_include_directories(rendertest PRIVATE
    thirdparty/cglm/include
    thirdparty/glad/include
)

target_link_libraries(rendertest PRIVATE
    X11 glad m
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
    -Wno-gnu-zero-variadic-macro
)
