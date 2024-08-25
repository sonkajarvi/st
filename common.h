#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#ifdef ST_PLATFORM_LINUX
#include <glad/glad_glx.h>
#elif defined(ST_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cglm/vec3.h>
#include <cglm/vec4.h>

/**
 * Platforms and backends:
 * - Windows
 *     - OpenGL (wgl)
 *     - Vulkan
 *     - DirectX (d11, d12)
 * - Linux (X11 and Wayland)
 *     - OpenGL (glx, egl)
 *     - Vulkan
 * - MacOS
 *     - OpenGL (cgl)
 *     - Vulkan
 *     - Metal
 */

#define call_impl(f, ...) \
    do { \
        assert(global_engine_context->impl.f); \
        global_engine_context->impl.f(__VA_ARGS__); \
    } while (0)

typedef struct StCamera
{
    vec3 position;
} StCamera;

typedef struct StWindow
{
    // keyboard
    // mouse
    
    bool is_open;

#ifdef ST_PLATFORM_LINUX
    struct {
        Display *display;
        GLXContext context;
        GLXFBConfig fbc;
        Window root, window;
    } x11;
#elif defined(ST_PLATFORM_WINDOWS)
    struct {
        HWND window;
    } win32;
#endif
} StWindow;

typedef struct StVertex
{
    vec3 position;
    vec4 color;
} StVertex;

typedef struct StEngine
{
    StWindow *window;

    // per platform functions
    struct {
        void (*window_create)(StWindow *, const char *, int, int);
        void (*window_destroy)(StWindow *);
        void (*window_show)(StWindow *);
        void (*window_get_size)(StWindow *, int *, int *);
        void (*window_get_pos)(StWindow *, int *, int *);

        void (*poll_events)(StWindow *);
        void (*swap_buffers)(StWindow *);
        
        void (*context_create)(StWindow *);
        void (*context_destroy)(StWindow *);
    
        void (*renderer_init)(StCamera *);
        void (*renderer_destroy)(void);
        void (*renderer_begin)(void);
        void (*renderer_end)(void);
        void (*renderer_push_mesh)(const StVertex *, const size_t, const unsigned int *, const size_t);
    } impl;
} StEngine;

extern StEngine *global_engine_context;

void engine_init(void);
void engine_destroy(void);

void window_create(const char *title, int width, int height);
void window_destroy(void);
void window_show(void);
bool window_should_close(void);
void window_get_size(int *width, int *height);
void window_get_pos(int *x, int *y);

void poll_events(void);
void swap_buffers(void);

void renderer_init(StCamera *camera);
void renderer_destroy(void);
void renderer_begin(void);
void renderer_end(void);
void renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count);

#endif // COMMON_H
