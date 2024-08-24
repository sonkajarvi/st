#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#include "common.h"

void impl_win32_window_create(struct st_window *, const char *, int, int);
void impl_win32_window_destroy(struct st_window *);
void impl_win32_window_show(struct st_window *);
void impl_win32_window_get_size(struct st_window *, int *, int *);
void impl_win32_window_get_pos(struct st_window *, int *, int *);

void impl_win32_poll_events(struct st_window *);
void impl_wgl_swap_buffers(struct st_window *);

void impl_wgl_context_create(struct st_window *);
void impl_wgl_context_destroy(struct st_window *);

#endif // WINDOW_WIN32_H
