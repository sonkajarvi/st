#include "common.h"

#include <stdlib.h>
#include <string.h>

#ifdef ST_PLATFORM_LINUX
#include "window_x11.h"
#elif defined(ST_PLATFORM_WINDOWS)
#include "window_win32.h"
#endif

#include "renderer_gl.h"

StEngine *global_engine_context = NULL;

#ifdef ST_PLATFORM_LINUX
static void impl_for_x11(void)
{
    global_engine_context->impl.window_create = impl_x11_window_create;
    global_engine_context->impl.window_destroy = impl_x11_window_destroy;
    global_engine_context->impl.window_show = impl_x11_window_show;
    global_engine_context->impl.window_get_size = impl_x11_window_get_size;
    global_engine_context->impl.window_get_pos = impl_x11_window_get_pos;

    global_engine_context->impl.poll_events = impl_x11_poll_events;
    global_engine_context->impl.swap_buffers = impl_glx_swap_buffers;

    global_engine_context->impl.context_create = impl_glx_context_create;
    global_engine_context->impl.context_destroy = impl_glx_context_destroy;

    global_engine_context->impl.renderer_init = impl_gl_renderer_init;
    global_engine_context->impl.renderer_destroy = impl_gl_renderer_destroy;
    global_engine_context->impl.renderer_begin = impl_gl_renderer_begin;
    global_engine_context->impl.renderer_end = impl_gl_renderer_end;
    global_engine_context->impl.renderer_push_mesh = impl_gl_renderer_push_mesh;
}
#elif defined(ST_PLATFORM_WINDOWS)
static void impl_for_win32(void)
{
    global_engine_context->impl.window_create = impl_win32_window_create;
    global_engine_context->impl.window_destroy = impl_win32_window_destroy;
    global_engine_context->impl.window_show = impl_win32_window_show;
    global_engine_context->impl.window_get_size = impl_win32_window_get_size;
    global_engine_context->impl.window_get_pos = impl_win32_window_get_pos;

    global_engine_context->impl.poll_events = impl_win32_poll_events;
    global_engine_context->impl.swap_buffers = impl_wgl_swap_buffers;

    global_engine_context->impl.context_create = impl_wgl_context_create;
    global_engine_context->impl.context_destroy = impl_wgl_context_destroy;

    global_engine_context->impl.renderer_init = impl_gl_renderer_init;
    global_engine_context->impl.renderer_destroy = impl_gl_renderer_destroy;
    global_engine_context->impl.renderer_begin = impl_gl_renderer_begin;
    global_engine_context->impl.renderer_end = impl_gl_renderer_end;
    global_engine_context->impl.renderer_push_mesh = impl_gl_renderer_push_mesh;
}
#endif

void engine_init(void)
{
    global_engine_context = malloc(sizeof(*global_engine_context));
    if (!global_engine_context)
        return;
    
    memset(global_engine_context, 0, sizeof(*global_engine_context));

#ifdef ST_PLATFORM_LINUX
    impl_for_x11();
#elif defined(ST_PLATFORM_WINDOWS)
    impl_for_win32();
#endif
}

void engine_destroy(void)
{
    free(global_engine_context);
}
