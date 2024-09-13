#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#include <stdbool.h>

#ifdef ST_PLATFORM_LINUX
#include <glad/glad_glx.h>
#elif defined(ST_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <st/input/keyboard.h>
#include <st/input/mouse.h>

typedef struct StWindow
{
    StMouse mouse;
    StKeyboard keyboard;
    
    bool is_open;
    float deltatime;

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
        UINT64 offset;
        UINT64 frequency;
    } win32;
#endif
} StWindow;

void window_create(const char *title, int width, int height);
void window_destroy(void);
void window_show(void);
bool window_should_close(void);

void window_get_size(int *width, int *height);
void window_get_pos(int *x, int *y);

double window_time(void);
int window_fps(void);
float window_deltatime(void);

void window_vsync(bool value);

#endif // ST_WINDOW_H
