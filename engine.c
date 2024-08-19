#include "common.h"

#include <stdlib.h>
#include <string.h>

#include "window_x11.h"
#include "renderer_gl.h"

struct st_engine *global_engine_context = NULL;

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

void engine_init(void)
{
    global_engine_context = malloc(sizeof(*global_engine_context));
    if (!global_engine_context)
        return;
    
    memset(global_engine_context, 0, sizeof(*global_engine_context));

    impl_for_x11();
}

void engine_destroy(void)
{
    free(global_engine_context);
}
