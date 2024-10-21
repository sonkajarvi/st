#ifndef ST_ENGINE_H
#define ST_ENGINE_H

#include <st/event.h>
#include <st/window.h>

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

// note: caller validates instance
#define call_impl(instance, callback, ...) \
    instance->impl.callback(__VA_ARGS__)

#define return_impl(f, ...) \
    do { \
        const StEngine *const e = st_engine_context(); \
        st_assert(e); \
        st_assert(e->impl.f); \
        return e->impl.f(__VA_ARGS__); \
    } while (0)

typedef struct St
{
    bool initialized;

    StWindow *window;

    StEventHandler event_handler;

    struct {
        double (*engine_time)(StWindow *);
        void (*window_create)(StWindow *, const char *, int, int);
        void (*window_destroy)(StWindow *);
        void (*window_show)(StWindow *);
        void (*window_get_size)(StWindow *, int *, int *);
        void (*window_get_pos)(StWindow *, int *, int *);
        void (*window_vsync)(StWindow *, bool);

        void (*poll_events)(StWindow *);
        void (*swap_buffers)(StWindow *);
    } impl;
} St;

void st_hello(void);
void st_goodbye(void);
St *st_instance(void);

#endif // ST_ENGINE_H
