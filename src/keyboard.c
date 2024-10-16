#include <assert.h>

#include <st/engine.h>
#include <st/input/keyboard.h>
#include <st/input/keys.h>

bool key_down(int key)
{
    assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return window->keyboard.state[key].current;
}

bool key_press(int key)
{
    assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return key_down(key) && !window->keyboard.state[key].previous;
}

bool key_release(int key)
{
    assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    assert(st);
    StWindow *window = st->window;
    assert(window);

    return !key_down(key) && window->keyboard.state[key].previous;
}
