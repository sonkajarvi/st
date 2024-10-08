#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#include <st/common.h>

double impl_win32_engine_time(StWindow *);

void impl_win32_window_create(StWindow *, const char *, int, int);
void impl_win32_window_destroy(StWindow *);
void impl_win32_window_show(StWindow *);
void impl_win32_window_get_size(StWindow *, int *, int *);
void impl_win32_window_get_pos(StWindow *, int *, int *);

void impl_win32_poll_events(StWindow *);
void impl_wgl_swap_buffers(StWindow *);

void impl_wgl_context_create(StWindow *);
void impl_wgl_context_destroy(StWindow *);
void impl_wgl_window_vsync(StWindow *, bool value);

#endif // WINDOW_WIN32_H
