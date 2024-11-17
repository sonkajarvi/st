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

// todo: Move elsewhere
#define ST_ENOST    1000 // Library not initialized
#define ST_EINIT    1001 // Already initialized

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

struct st
{
    bool init;

    // Pointer to the created window, set by `st_window_create()`
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
};

// st_hello() - Initialize the library instance
int st_hello(void);

// st_goodbye() - Deinitialize the library instance
void st_goodbye(void);

// st_instance() - Get the library instance
//
// Returns: A pointer to the initialized instance, NULL otherwise
struct st *st_instance(void);

// Called once at program start up
void st_set_platform_callbacks(struct st *const st);
// Called at least once at program start up, and anytime after that
void st_set_graphics_callbacks(struct st *const st, int type);

#endif // ST_ENGINE_H
