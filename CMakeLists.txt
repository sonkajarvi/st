cmake_minimum_required(VERSION 3.20)

project(rendertest LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/engine.c
    src/camera.c
    src/glad_loader.c
    src/keyboard.c
    src/model.c
    src/mouse.c
    src/parse.c
    src/renderer.c
    src/renderer_gl.c
    src/util.c
    src/vector.c
    src/window.c
)

set(LIBS glad m)

if (WIN32)
    message("using Windows")
    add_compile_definitions(ST_PLATFORM_WINDOWS)
    set(LIBS ${LIBS} gdi32 opengl32)
    set(SOURCES ${SOURCES} src/window_win32.c)
    
elseif (UNIX AND NOT APPLE)
    message("using Linux")
    add_compile_definitions(ST_PLATFORM_LINUX)
    set(LIBS ${LIBS} X11)
    set(SOURCES ${SOURCES} src/window_x11.c)

endif()


add_library(rendertest ${SOURCES})

# Absolute assets path
target_compile_definitions(rendertest PUBLIC ST_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_subdirectory(thirdparty/glad)

target_include_directories(rendertest PRIVATE
    include
    thirdparty/cglm/include
    thirdparty/glad/include
)

target_link_libraries(rendertest PRIVATE ${LIBS})

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

# tests
enable_testing()
add_subdirectory(tests)
