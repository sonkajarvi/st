#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#include <assert.h>
#include <stdbool.h>

#ifdef ST_PLATFORM_LINUX
#include <glad/glx.h>
#endif

#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/input/keyboard.h>
#include <st/input/mouse.h>

typedef struct StWindow
{
    bool visible;
    float deltatime;

    StMouse mouse;
    StKeyboard keyboard;
    StCamera camera;
    StRenderer renderer;

#ifdef ST_PLATFORM_LINUX
    struct {
        // todo: separate glx from x11
        Display *display;
        GLXContext context;
        GLXFBConfig fbc;
        Window root;
        Window window;
        uint64_t offset;
    } x11;
#elif defined(ST_PLATFORM_WINDOWS)
    struct {
        HWND window;
        UINT64 offset;
        UINT64 frequency;
    } win32;
#endif
} StWindow;

StWindow *st_window_create(const char *title, int width, int height);
void st_window_destroy(StWindow *window);
void st_window_show(StWindow *window);
bool st_window_should_close(StWindow *window);

void st_window_get_size(int *width, int *height);
void st_window_get_pos(int *x, int *y);

double st_window_time(void);
// int window_fps(void);
float st_window_deltatime(void);

void st_window_poll_events(StWindow *window);
void st_window_swap_buffers(StWindow *window);
void st_window_set_vsync(StWindow *window, bool value);

static inline StCamera *st_window_camera(StWindow *const window)
{
    assert(window);
    return &window->camera;
}

static inline StRenderer *st_window_renderer(StWindow *const window)
{
    assert(window);
    return &window->renderer;
}

#endif // ST_WINDOW_H
