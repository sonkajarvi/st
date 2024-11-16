#ifndef IMPL_CONTEXT_GLX_H
#define IMPL_CONTEXT_GLX_H

#include <st/window.h>

void glx_init(Display *display, int screen);
void glx_create_context(struct st_window *window);
void glx_destroy_context(struct st_window *window);
void glx_choose_fbc(Display *display, int screen, GLXFBConfig *config);

void impl_glx_swap_buffers(struct st_window *window);
void impl_glx_window_vsync(struct st_window *window, bool on);

#endif // IMPL_CONTEXT_GLX_H
