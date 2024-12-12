cmake_minimum_required(VERSION 3.20)

project(st LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/assert.c
    src/camera.c
    src/callbacks.c
    src/draw_text.c
    src/instance.c
    src/keyboard.c
    src/mouse.c
    src/print.c
    src/renderer.c
    src/texture.c
    src/util.c
    src/vector.c
    src/window.c

    src/image/image.c

    src/impl/renderer_gl.c
    src/impl/texture_gl.c
)

set(LIBS glad m)

if (UNIX AND NOT APPLE)
    message("st: Building for Linux")
    set(LIBS ${LIBS} X11 Xfixes)
    set(SOURCES ${SOURCES} src/impl/window_x11.c)
    set(SOURCES ${SOURCES} src/impl/context_glx.c)
endif()

add_library(st ${SOURCES})

if (UNIX AND NOT APPLE)
    target_compile_definitions(st PUBLIC ST_PLATFORM_LINUX)
endif()

execute_process(
    COMMAND git log -1 --pretty=%H
    OUTPUT_VARIABLE COMMIT_HASH
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
    COMMAND git log -1 --pretty=%s
    OUTPUT_VARIABLE COMMIT_SUBJECT
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Absolute assets path
target_compile_definitions(st PUBLIC
    ST_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
    ST_COMMIT_HASH="${COMMIT_HASH}"
    ST_COMMIT_SUBJECT="${COMMIT_SUBJECT}"
)

add_subdirectory(thirdparty/glad)

target_include_directories(st PRIVATE
    include
    thirdparty/cglm/include
    thirdparty/glad/include
    thirdparty/stb/include
)

target_link_libraries(st PRIVATE ${LIBS})

target_compile_options(st PRIVATE
    -g
    -O3
    -Wall
    -Wextra
    -Werror
    # -Wstrict-prototypes
    # -Wno-gnu-zero-variadic-macro
)

# tests
enable_testing()
add_subdirectory(tests)
