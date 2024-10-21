#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <st/engine.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/utility/print.h>

// todo: return a status code
StWindow *st_window_create(const char *title, int width, int height)
{
    St *st = st_instance();
    if (!st) {
        st_error("Failed to create window. No instance found\n");
        return NULL;
    }

    if (st->window) {
        st_error("Failed to create window. Window already exists\n");
        return NULL;
    }

    StWindow *window = malloc(sizeof(*window));
    if (!window) {
        st_error("Failed to allocate memory for window\n");
        return NULL;
    }

    memset(window, 0, sizeof(*window));
    call_impl(st, window_create, window, title, width, height);
    st->window = window;

    st_camera_init(&st->window->camera, ST_CAMERA_ORTHO);
    st_renderer_init(&st->window->renderer, &st->window->camera);

    st_debug("Window created\n");
    st_debug("... title: '%s'\n", title);
    st_debug("... size: %dx%d\n", width, height);

    return window;
}

void st_window_destroy(StWindow *window)
{
    assert(window);
    St *st = st_instance();
    assert(st);

    st_renderer_destroy(&window->renderer);

    // call_impl(e, context_destroy, window);
    call_impl(st, window_destroy, window);

    // todo: destroy the graphics context

    free(window);

    st_debug("Window destroyed\n");
}

void st_window_show(StWindow *window)
{
    assert(window);
    St *st = st_instance();
    assert(st);

    call_impl(st, window_show, window);

    window->visible = true;
}

bool st_window_should_close(StWindow *window)
{
    assert(window);
    St *st = st_instance();
    assert(st);

    return !window->visible;
}

void st_window_get_size(int *width, int *height)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    call_impl(st, window_get_size, window, width, height);
}

void st_window_get_pos(int *x, int *y)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    call_impl(st, window_get_pos, window, x, y);
}

double st_window_time(void)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    // return_impl(engine_time, window);
    return call_impl(st, engine_time, window);
}

// int window_fps(void)
// {
//     StEngine *e = st_instance();
//     assert(e);
//     StWindow *window = e->window;
//     assert(window);

//     return window->fps;
// }

float st_window_deltatime(void)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return window->deltatime;
}

void st_window_set_vsync(StWindow *window, bool value)
{
    assert(window);
    St *st = st_instance();
    assert(st);

    call_impl(st, window_vsync, window, value);
}

void st_window_poll_events(StWindow *window)
{
    St *st = st_instance();
    assert(st);

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

    call_impl(st, poll_events, window);
}

void st_window_swap_buffers(StWindow *window)
{
    St *st = st_instance();
    assert(st);

    call_impl(st, swap_buffers, window);
}
