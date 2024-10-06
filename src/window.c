#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <st/engine.h>
#include <st/utils/print.h>

// todo: return a status code
void st_window_create(const char *title, int width, int height)
{
    StEngine *engine = st_engine_context();
    if (!engine) {
        st_error("No engine context\n");
        return;
    }

    if (engine->window) {
        st_error("Window already initialized\n");
        return;
    }

    StWindow *window = malloc(sizeof(*window));
    if (!window) {
        st_error("Failed to allocate memory for window struct\n");
        return;
    }

    memset(window, 0, sizeof(*window));
    call_impl(engine, window_create, window, title, width, height);
    engine->window = window;

    st_debug("Window created (\"%s\", %d, %d)\n", title, width, height);
}

void st_window_destroy(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    // call_impl(e, context_destroy, window);
    call_impl(e, window_destroy, window);

    // todo: destroy the graphics context

    free(window);

    st_debug("Window destroyed\n");
}

void st_window_show(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(e, window_show, window);

    window->visible = true;
}

bool st_window_should_close(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return !window->visible;
}

void st_window_get_size(int *width, int *height)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(e, window_get_size, window, width, height);
}

void st_window_get_pos(int *x, int *y)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(e, window_get_pos, window, x, y);
}

double st_window_time(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    // return_impl(engine_time, window);
    return call_impl(e, engine_time, window);
}

// int window_fps(void)
// {
//     StEngine *e = st_engine_context();
//     assert(e);
//     StWindow *window = e->window;
//     assert(window);

//     return window->fps;
// }

float st_window_deltatime(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return window->deltatime;
}

void st_window_vsync(bool value)
{
    (void)value;
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(e, window_vsync, window, value);
}

void st_window_poll_events(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    // update time
    // static int frames = 0;
    // static double elapsed = 0.0;
    static double last = 0.0;

    double now = st_window_time();
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

    call_impl(e, poll_events, window);
}

void st_window_swap_buffers(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(e, swap_buffers, window);
}
