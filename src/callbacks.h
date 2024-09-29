#ifndef CALLBACKS_H
#define CALLBACKS_H

// todo: move elsewhere
#define ST_GRAPHICS_OPENGL 1

#include <st/engine.h>
#include <st/print.h>
#include <st/renderer_gl.h>

#ifdef ST_PLATFORM_LINUX
#include "impl/window_x11.h"
#include "impl/context_glx.h"
#endif // ST_PLATFORM_LINUX

// called once, at program start up
// note: caller validates engine context
static inline void set_platform_callbacks(StEngine *const e)
{
#ifdef ST_PLATFORM_LINUX
    e->impl.window_create               = impl_x11_window_create;
    e->impl.window_destroy              = impl_x11_window_destroy;
    e->impl.window_show                 = impl_x11_window_show;
    e->impl.window_get_size             = impl_x11_window_get_size;
    e->impl.window_get_pos              = impl_x11_window_get_pos;
    e->impl.engine_time                 = impl_x11_window_time;
    e->impl.poll_events                 = impl_x11_poll_events;

    st_debug("Platform callbacks set for X11\n");
#endif // ST_PLATFORM_LINUX
}

// called at least once, at program start up and after
// note: caller validates engine context
static inline void set_graphics_callbacks(StEngine *const e, int type)
{
    switch (type) {
    case ST_GRAPHICS_OPENGL:
        e->impl.renderer_init           = impl_gl_renderer_init;
        e->impl.renderer_destroy        = impl_gl_renderer_destroy;
        e->impl.renderer_begin          = impl_gl_renderer_begin;
        e->impl.renderer_end            = impl_gl_renderer_end;
        e->impl.renderer_push_mesh      = impl_gl_renderer_push_mesh;
        e->impl.renderer_push_model     = impl_gl_renderer_draw_model;

#ifdef ST_PLATFORM_LINUX
        e->impl.swap_buffers            = impl_glx_swap_buffers;
        e->impl.window_vsync            = impl_glx_window_vsync;
#endif // ST_PLATFORM_LINUX

        st_debug("Graphics callbacks set for OpenGL\n");
        break;
    }
}

#endif // CALLBACKS_H
