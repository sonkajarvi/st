#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#include <stdbool.h>

#ifdef ST_PLATFORM_LINUX
#include <glad/glx.h>
#endif

#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/input/keyboard.h>
#include <st/input/mouse.h>
#include <st/utility/assert.h>

struct st_window
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
};

struct st_window *st_window_create(const char *title, int width, int height);
void st_window_destroy(struct st_window *window);
void st_window_show(struct st_window *window);
bool st_window_should_close(struct st_window *window);

void st_window_get_size(int *width, int *height);
void st_window_get_pos(int *x, int *y);

double st_window_time(void);
// int window_fps(void);
float st_window_deltatime(void);

void st_window_poll_events(struct st_window *window);
void st_window_swap_buffers(struct st_window *window);
void st_window_set_vsync(struct st_window *window, bool value);

#endif // ST_WINDOW_H
