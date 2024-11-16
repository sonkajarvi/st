#include <st/instance.h>
#include <st/window.h>
#include <st/input/mouse.h>
#include <st/utility/assert.h>

void mouse_get_pos(int *x, int *y)
{
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    if (x)
        *x = window->mouse.position[0];
    if (y)
        *y = window->mouse.position[1];
}

void mouse_get_delta(int *x, int *y)
{
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    if (x)
        *x = window->mouse.delta[0];
    if (y)
        *y = window->mouse.delta[1];
}

float mouse_get_wheel(void)
{
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    return window->mouse.wheel;
}

bool mouse_down(int button)
{
    st_assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    return window->mouse.state[button].current;
}

bool mouse_press(int button)
{
    st_assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    return mouse_down(button) && !window->mouse.state[button].previous;
}

bool mouse_release(int button)
{
    st_assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    return !mouse_down(button) && window->mouse.state[button].previous;
}
