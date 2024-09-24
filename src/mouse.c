#include <assert.h>

#include <st/engine.h>
#include <st/input/mouse.h>
#include <st/window.h>

void mouse_get_pos(int *x, int *y)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    if (x)
        *x = window->mouse.position[0];
    if (y)
        *y = window->mouse.position[1];
}

void mouse_get_delta(int *x, int *y)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    if (x)
        *x = window->mouse.delta[0];
    if (y)
        *y = window->mouse.delta[1];
}

float mouse_get_wheel(void)
{
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return window->mouse.wheel;
}

bool mouse_down(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return window->mouse.state[button].current;
}

bool mouse_press(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return mouse_down(button) && !window->mouse.state[button].previous;
}

bool mouse_release(int button)
{
    assert(button >= __ST_MOUSE_FIRST && button <= __ST_MOUSE_LAST);
    StEngine *e = st_engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    return !mouse_down(button) && window->mouse.state[button].previous;
}
