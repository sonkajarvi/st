#ifndef WINDOW_X11_H
#define WINDOW_X11_H

#include <st/window.h>

void impl_x11_window_create(struct st_window *, const char *, int, int);
void impl_x11_window_destroy(struct st_window *);
void impl_x11_window_show(struct st_window *);

void impl_x11_window_get_size(struct st_window *, int *, int *);
void impl_x11_window_get_pos(struct st_window *, int *, int *);
double impl_x11_window_time(struct st_window *);

void impl_x11_poll_events(struct st_window *);

void impl_x11_window_show_cursor(struct st_window *win, bool shown);

#endif // WINDOW_X11_H
