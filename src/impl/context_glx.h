#ifndef IMPL_CONTEXT_GLX_H
#define IMPL_CONTEXT_GLX_H

#include <st/window.h>

void glx_init(Display *display, int screen);
void glx_create_context(StWindow *window);
void glx_destroy_context(StWindow *window);
void glx_choose_fbc(Display *display, int screen, GLXFBConfig *config);

void impl_glx_swap_buffers(StWindow *window);
void impl_glx_window_vsync(StWindow *window, bool on);

#endif // IMPL_CONTEXT_GLX_H
