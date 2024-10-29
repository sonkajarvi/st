#ifndef CALLBACKS_H
#define CALLBACKS_H

// todo: move elsewhere
#define ST_GRAPHICS_OPENGL 1

#include <st/instance.h>
#include <st/utility/print.h>

#ifdef ST_PLATFORM_LINUX
#include "impl/window_x11.h"
#include "impl/context_glx.h"
#include "impl/renderer_gl.h"
#endif

// Called once at program start up
// note: caller validates engine context
static inline void set_platform_callbacks(St *const st)
{
#ifdef ST_PLATFORM_LINUX
    st->impl.window_create   = impl_x11_window_create;
    st->impl.window_destroy  = impl_x11_window_destroy;
    st->impl.window_show     = impl_x11_window_show;
    st->impl.window_get_size = impl_x11_window_get_size;
    st->impl.window_get_pos  = impl_x11_window_get_pos;
    st->impl.engine_time     = impl_x11_window_time;
    st->impl.poll_events     = impl_x11_poll_events;

    st_debug("Platform callbacks set for X11\n");
#endif
}

// Called at least once at program start up, and anytime after that
// note: caller validates engine context
static inline void set_graphics_callbacks(St *const st, int type)
{
    switch (type) {
    case ST_GRAPHICS_OPENGL:
#ifdef ST_PLATFORM_LINUX
        st->impl.swap_buffers = impl_glx_swap_buffers;
        st->impl.window_vsync = impl_glx_window_vsync;
#endif
        st->impl.renderer_init    = impl_gl_renderer_init;
        st->impl.renderer_destroy = impl_gl_renderer_destroy;
        st->impl.renderer_add_texture = impl_gl_renderer_add_texture;
        st->impl.draw_begin       = impl_gl_draw_begin;
        st->impl.draw_end         = impl_gl_draw_end;
        st->impl.renderer_push    = impl_gl_renderer_push;

        st_debug("Graphics callbacks set for OpenGL\n");
        break;
    }
}

#endif // CALLBACKS_H
