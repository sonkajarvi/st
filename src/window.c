#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <st/engine.h>

void window_create(const char *title, int width, int height)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow **window = &e->window;

    *window = malloc(sizeof(**window));
    assert(*window);
    memset(*window, 0, sizeof(**window));

    call_impl(window_create, *window, title, width, height);
    call_impl(context_create, *window);
}

void window_destroy(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(context_destroy, window);
    call_impl(window_destroy, window);

    free(window);
}

void window_show(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(window_show, window);

    window->is_open = true;
}

bool window_should_close(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return !window->is_open;    
}

void window_get_size(int *width, int *height)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(window_get_size, window, width, height);
}

void window_get_pos(int *x, int *y)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(window_get_pos, window, x, y);    
}

double window_time(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return_impl(engine_time, window);
}

// int window_fps(void)
// {
//     StEngine *e = engine_context();
//     assert(e);
//     StWindow *window = e->window;
//     assert(window);

//     return window->fps;
// }

float window_deltatime(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return window->deltatime;
}

void window_vsync(bool value)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(window_vsync, window, value);
}

void poll_events(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    // update time
    // static int frames = 0;
    // static double elapsed = 0.0;
    static double last = 0.0;

    double now = window_time();
    window->deltatime = (float)(now - last);
    // elapsed += now - last;
    last = now;

    // if (elapsed >= 1.0) {
    //     elapsed = 0.0;
    //     window->fps = frames;
    //     frames = 0;
    // }
    // frames++;

    // reset mouse wheel delta
    window->mouse.wheel = 0.0f;

    // update mouse button states
    for (int i = 0; i < __ST_MOUSE_COUNT; i++)
        window->mouse.state[i].previous = window->mouse.state[i].current;

    // update mouse position delta
    static ivec2 prev_position = {0, 0};
    window->mouse.delta[0] = window->mouse.position[0] - prev_position[0];
    window->mouse.delta[1] = window->mouse.position[1] - prev_position[1];
    prev_position[0] = window->mouse.position[0];
    prev_position[1] = window->mouse.position[1];

    // update keyboard key states
    for (int i = 0; i < __ST_KEY_COUNT; i++)
        window->keyboard.state[i].previous = window->keyboard.state[i].current;

    call_impl(poll_events, window);
}

void swap_buffers(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(swap_buffers, window);
}
