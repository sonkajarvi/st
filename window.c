#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void window_create(const char *title, int width, int height)
{
    struct st_window **window = &global_engine_context->window;

    *window = malloc(sizeof(**window));
    assert(*window);
    memset(*window, 0, sizeof(**window));

    call_impl(window_create, *window, title, width, height);
    call_impl(context_create, *window);
}

void window_destroy(void)
{
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(context_destroy, window);
    call_impl(window_destroy, window);

    free(window);
}

void window_show(void)
{
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(window_show, window);

    window->is_open = true;
}

bool window_should_close(void)
{
    struct st_window *window = global_engine_context->window;
    assert(window);

    return !window->is_open;    
}

void poll_events(void)
{
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(poll_events, window);
}

void swap_buffers(void)
{
    struct st_window *window = global_engine_context->window;
    assert(window);

    call_impl(swap_buffers, window);
}
