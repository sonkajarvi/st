#include <st/engine.h>
#include <st/input/keyboard.h>
#include <st/input/keys.h>
#include <st/utility/assert.h>

bool key_down(int key)
{
    st_assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    st_assert(st);
    StWindow *window = st->window;
    st_assert(window);

    return window->keyboard.state[key].current;
}

bool key_press(int key)
{
    st_assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    st_assert(st);
    StWindow *window = st->window;
    st_assert(window);

    return key_down(key) && !window->keyboard.state[key].previous;
}

bool key_release(int key)
{
    st_assert(key >= __ST_KEY_FIRST && key <= __ST_KEY_LAST);
    St *st = st_instance();
    st_assert(st);
    StWindow *window = st->window;
    st_assert(window);

    return !key_down(key) && window->keyboard.state[key].previous;
}
