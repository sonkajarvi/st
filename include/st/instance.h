#ifndef ST_ENGINE_H
#define ST_ENGINE_H

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

// todo: Move elsewhere
#define ST_GRAPHICS_OPENGL 1

// note: caller validates instance
#define call_impl(instance, callback, ...) \
    instance->impl.callback(__VA_ARGS__)

#define return_impl(f, ...) \
    do { \
        const StEngine *const e = st_engine_context(); \
        assert(e); \
        assert(e->impl.f); \
        return e->impl.f(__VA_ARGS__); \
    } while (0)

typedef struct St
{
    bool initialized;

    struct st_window *window;

    struct {
        double (*engine_time)(struct st_window *);
        void (*window_create)(struct st_window *, const char *, int, int);
        void (*window_destroy)(struct st_window *);
        void (*window_show)(struct st_window *);
        void (*window_get_size)(struct st_window *, int *, int *);
        void (*window_get_pos)(struct st_window *, int *, int *);
        void (*window_vsync)(struct st_window *, bool);

        void (*poll_events)(struct st_window *);
        void (*swap_buffers)(struct st_window *);

        void (*renderer_init)(struct st_window *, StRenderer *, StCamera *);
        void (*renderer_destroy)(struct st_window *, StRenderer *);

        void (*texture_create)(struct st_texture *, struct st_image *);
        void (*texture_destroy)(struct st_texture *);

        void (*draw_begin)(struct st_window *, StRenderer *);
        void (*draw_end)(struct st_window *, StRenderer *);

        void (*renderer_push)(struct st_window *, StRenderer *, StVertex *, size_t);
    } impl;
} St;

void st_hello(void);
void st_goodbye(void);
St *st_instance(void);

// Called once at program start up
void st_set_platform_callbacks(St *const st);
// Called at least once at program start up, and anytime after that
void st_set_graphics_callbacks(St *const st, int type);

#endif // ST_ENGINE_H
