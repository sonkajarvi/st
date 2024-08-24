cmake_minimum_required(VERSION 3.20)

project(rendertest LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(SOURCES
    engine.c
    glad_loader.c
    renderer.c
    renderer_gl.c
    util.c
    window.c
)

set(LIBS glad m)

if (DEFINED WIN32)
    message("using Windows")
    add_compile_definitions(rendertest PRIVATE ST_PLATFORM_WINDOWS)
    set(LIBS ${LIBS} gdi32 opengl32)
    set(SOURCES ${SOURCES} window_win32.c)
    
elseif (DEFINED __linux__)
    message("using Linux")
    add_compile_definitions(rendertest PRIVATE ST_PLATFORM_LINUX)
    set(LIBS ${LIBS} X11)
    set(SOURCES ${SOURCES} window_x11.c)

endif()

add_library(rendertest ${SOURCES})

add_subdirectory(thirdparty/glad)

target_include_directories(rendertest PRIVATE
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
