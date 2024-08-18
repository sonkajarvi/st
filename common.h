#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#include <GL/glx.h>
#include <X11/Xlib.h>

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

struct st_window
{
    // keyboard
    // mouse
    
    bool is_open;

    struct {
        Display *display;
        GLXContext context;
        GLXFBConfig fbc;
        Window root, window;
    } x11;
};

struct st_engine
{
    struct st_window *window;

    // per platform functions
    struct {
        void (*window_create)(struct st_window*, const char *, int, int);
        void (*window_destroy)(struct st_window*);
        void (*window_show)(struct st_window*);
        void (*window_get_size)(struct st_window*, int *, int *);
        void (*window_get_pos)(struct st_window*, int *, int *);

        void (*poll_events)(struct st_window*);
        void (*swap_buffers)(struct st_window*);
        
        void (*context_create)(struct st_window*);
        void (*context_destroy)(struct st_window*);
    } impl;
};

extern struct st_engine *global_engine_context;

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

#endif // COMMON_H
