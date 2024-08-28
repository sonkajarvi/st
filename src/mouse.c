#include <assert.h>

#include <st/common.h>

void mouse_get_pos(int *x, int *y)
{
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    if (x)
        *x = window->mouse.position[0];
    if (y)
        *y = window->mouse.position[1];
}

void mouse_get_delta(int *x, int *y)
{
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    if (x)
        *x = window->mouse.delta[0];
    if (y)
        *y = window->mouse.delta[1];
}

float mouse_get_wheel(void)
{
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return window->mouse.wheel;
}

bool mouse_down(int button)
{
    assert(button >= ST_MOUSE_FIRST && button <= ST_MOUSE_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return window->mouse.state[button].curr;
}

bool mouse_press(int button)
{
    assert(button >= ST_MOUSE_FIRST && button <= ST_MOUSE_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return mouse_down(button) && !window->mouse.state[button].prev;
}

bool mouse_release(int button)
{
    assert(button >= ST_MOUSE_FIRST && button <= ST_MOUSE_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return !mouse_down(button) && window->mouse.state[button].prev;
}
