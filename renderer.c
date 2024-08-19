#include "common.h"

#include <assert.h>

void renderer_init(void)
{
    assert(global_engine_context);
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(renderer_init);
}

void renderer_destroy(void)
{
    assert(global_engine_context);
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(renderer_destroy);
}

void renderer_begin(void)
{
    assert(global_engine_context);
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(renderer_begin);
}

void renderer_end(void)
{
    assert(global_engine_context);
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(renderer_end);
}

void renderer_push_mesh(const struct st_vertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count)
{
    assert(global_engine_context);
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(renderer_push_mesh, vertices, vertex_count, indices, index_count);
}
