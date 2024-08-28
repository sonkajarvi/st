#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#ifdef ST_PLATFORM_LINUX
#include <glad/glad_glx.h>
#elif defined(ST_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cglm/ivec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include <st/keys.h>

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

#define return_impl(f, ...) \
    do { \
        assert(global_engine_context->impl.f); \
        return global_engine_context->impl.f(__VA_ARGS__); \
    } while (0)

#define ST_MOUSE_LEFT   0
#define ST_MOUSE_RIGHT  1
#define ST_MOUSE_MIDDLE 2
#define ST_MOUSE_X1     3
#define ST_MOUSE_X2     4

#define ST_MOUSE_FIRST  ST_MOUSE_LEFT
#define ST_MOUSE_LAST   ST_MOUSE_X2
#define ST_MOUSE_COUNT  (ST_MOUSE_LAST + 1)

typedef struct StCamera
{
    vec3 position;
} StCamera;

typedef struct StLight
{
    vec3 position;
    vec3 color;
} StLight;

typedef struct StWindow
{
    struct {
        ivec2 position, delta;
        struct {
            unsigned char curr : 1;
            unsigned char prev : 1;
        } state[ST_MOUSE_COUNT];
        float wheel;
    } mouse;

    struct {
        struct {
            unsigned char curr : 1;
            unsigned char prev : 1;
        } state[ST_KEY_COUNT];
    } keyboard;

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
    vec3 normal;
} StVertex;

typedef struct StModel
{
    StVertex *vertices;
} StModel;

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
    
        void (*renderer_init)(StCamera *, StLight *);
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

void mouse_get_pos(int *x, int *y);
void mouse_get_delta(int *x, int *y);
float mouse_get_wheel(void);
bool mouse_down(int button);
bool mouse_press(int button);
bool mouse_release(int button);

bool key_down(int key);
bool key_press(int key);
bool key_release(int key);

void renderer_init(StCamera *camera, StLight *light);
void renderer_destroy(void);
void renderer_begin(void);
void renderer_end(void);
void renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count);

void model_from_obj(StModel *model, const char *path);
void model_destroy(StModel *model);

#endif // COMMON_H
