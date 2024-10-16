#include <assert.h>

#include <st/engine.h>
#include <st/input/mouse.h>
#include <st/window.h>

void mouse_get_pos(int *x, int *y)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    if (x)
        *x = window->mouse.position[0];
    if (y)
        *y = window->mouse.position[1];
}

void mouse_get_delta(int *x, int *y)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    if (x)
        *x = window->mouse.delta[0];
    if (y)
        *y = window->mouse.delta[1];
}

float mouse_get_wheel(void)
{
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return window->mouse.wheel;
}

bool mouse_down(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return window->mouse.state[button].current;
}

bool mouse_press(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return mouse_down(button) && !window->mouse.state[button].previous;
}

bool mouse_release(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return !mouse_down(button) && window->mouse.state[button].previous;
}
