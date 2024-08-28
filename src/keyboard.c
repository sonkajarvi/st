#include <assert.h>

#include <st/common.h>
#include <st/keys.h>

bool key_down(int key)
{
    assert(key >= ST_KEY_FIRST && key <= ST_KEY_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return window->keyboard.state[key].curr;
}

bool key_press(int key)
{
    assert(key >= ST_KEY_FIRST && key <= ST_KEY_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return key_down(key) && !window->keyboard.state[key].prev;
}

bool key_release(int key)
{
    assert(key >= ST_KEY_FIRST && key <= ST_KEY_LAST);
    assert(global_engine_context);
    StWindow *window = global_engine_context->window;
    assert(window);

    return !key_down(key) && window->keyboard.state[key].prev;
}
