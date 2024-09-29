#ifndef ST_ENGINE_H
#define ST_ENGINE_H

#include <st/camera.h>
#include <st/model.h>
#include <st/renderer.h>
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

// note: caller validates engine context
#define call_impl(engine, callback, ...) \
    engine->impl.callback(__VA_ARGS__)

#define return_impl(f, ...) \
    do { \
        const StEngine *const e = st_engine_context(); \
        assert(e); \
        assert(e->impl.f); \
        return e->impl.f(__VA_ARGS__); \
    } while (0)

typedef struct StEngine
{
    StWindow *window;
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
    
        void (*renderer_init)(StCamera *, StLight *);
        void (*renderer_destroy)(void);
        void (*renderer_begin)(void);
        void (*renderer_end)(void);
        void (*renderer_push_mesh)(const StVertex *, const size_t, const unsigned int *, const size_t);
        void (*renderer_push_model)(StModel *, mat4 *, vec4, StCamera *, StLight *);
    } impl;
} StEngine;

void st_engine_init(void);
void st_engine_destroy(void);
StEngine *st_engine_context(void);

#endif // ST_ENGINE_H
