#include <errno.h>
#include <string.h>

#include <st/instance.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/input/mouse.h>
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

    st_log("Window created at %p (title=\"%s\", width=%d, height=%d)\n",
        win, title, width, height);

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

void st_window_show_cursor(struct st_window *win, bool shown)
{
    st_assert(win);
    struct st *st = st_instance();
    st_assert(st);

    call_impl(st, window_show_cursor, win, shown);
}

void st_window_poll_events(struct st_window *win)
{
    struct st *st = st_instance();
    st_assert(st);

    // update time
    // static int frames = 0;
    // static double elapsed = 0.0;
    static double last = 0.0;

    double now = st_window_time();
    win->deltatime = (float)(now - last);
    // elapsed += now - last;
    last = now;

    // if (elapsed >= 1.0) {
    //     elapsed = 0.0;
    //     window->fps = frames;
    //     frames = 0;
    // }
    // frames++;

    // Update mouse button states
    for (int i = __ST_MOUSE_FIRST; i <= __ST_MOUSE_LAST; i++)
        win->mouse.states[i].prev = win->mouse.states[i].curr;

    // Update mouse position delta
    static int prev_mouse_X = 0, prev_mouse_y = 0;
    win->mouse.dx = win->mouse.x - prev_mouse_X;
    win->mouse.dx = win->mouse.y - prev_mouse_y;
    prev_mouse_X = win->mouse.x;
    prev_mouse_y = win->mouse.y;

    // Reset mouse wheel delta
    win->mouse.wheel = 0.0f;

    // update keyboard key states
    for (int i = 0; i < __ST_KEY_COUNT; i++)
        win->keyboard.state[i].previous = win->keyboard.state[i].current;

    call_impl(st, poll_events, win);
}

void st_window_swap_buffers(struct st_window *window)
{
    struct st *st = st_instance();
    st_assert(st);

    call_impl(st, swap_buffers, window);
}
