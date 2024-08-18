#ifndef WINDOW_X11_H
#define WINDOW_X11_H

#include "common.h"

void impl_x11_window_create(struct st_window *, const char *, int, int);
void impl_x11_window_destroy(struct st_window *);
void impl_x11_window_show(struct st_window *);
void impl_x11_window_get_size(struct st_window *, int *, int *);
void impl_x11_window_get_pos(struct st_window *, int *, int *);

void impl_x11_poll_events(struct st_window *);
void impl_glx_swap_buffers(struct st_window *);

void impl_glx_context_create(struct st_window *);
void impl_glx_context_destroy(struct st_window *);

#endif // WINDOW_X11_H
