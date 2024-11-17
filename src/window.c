#include <errno.h>
#include <string.h>

#include <st/instance.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>

int st_window_create(struct st_window *win, const char *title, int width, int height)
{
    struct st *st;
    if (!(st = st_instance()))
        return ST_ENOST;

    if (!win || !title || !title[0] || width <= 0 || height <= 0)
        return EINVAL;

    memset(win, 0, sizeof(*win));
    st->window = win;

    call_impl(st, window_create, win, title, width, height);
    st_camera_init(&win->camera, ST_CAMERA_ORTHO);
    st_renderer_init(&win->renderer, &win->camera);

    st_log("Window created at %p (title=\"%s\", width=%d, height=%d)\n", win, title, width, height);

    return 0;
}

void st_window_destroy(struct st_window *win)
{
    struct st *st;
    if (!win || !(st = st_instance()))
        return;

    st_renderer_destroy(&win->renderer);
    call_impl(st, window_destroy, win);

    st_log("Window destroyed at %p\n", win);
}

void st_window_show(struct st_window *window)
{
    st_assert(window);
    struct st *st = st_instance();
    st_assert(st);

    call_impl(st, window_show, window);

    window->visible = true;
}

bool st_window_should_close(struct st_window *window)
{
    st_assert(window);
    struct st *st = st_instance();
    st_assert(st);

    return !window->visible;
}

void st_window_get_size(int *width, int *height)
{
    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    call_impl(st, window_get_size, window, width, height);
}

void st_window_get_pos(int *x, int *y)
{
    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    call_impl(st, window_get_pos, window, x, y);
}

double st_window_time(void)
{
    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    // return_impl(engine_time, window);
    return call_impl(st, engine_time, window);
}

// int window_fps(void)
// {
//     StEngine *e = st_instance();
//     st_assert(e);
//     struct st_window *window = e->window;
//     st_assert(window);

//     return window->fps;
// }

float st_window_deltatime(void)
{
    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    return window->deltatime;
}

void st_window_set_vsync(struct st_window *window, bool value)
{
    st_assert(window);
    struct st *st = st_instance();
    st_assert(st);

    call_impl(st, window_vsync, window, value);
}

void st_window_poll_events(struct st_window *window)
{
    struct st *st = st_instance();
    st_assert(st);

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

void st_window_swap_buffers(struct st_window *window)
{
    struct st *st = st_instance();
    st_assert(st);

    call_impl(st, swap_buffers, window);
}
