#include <st/input/mouse.h>
#include <st/util/print.h>

bool st_mouse_down(struct st_mouse *m, int btn)
{
    if (!m)
        return false;

    if (btn < __ST_MOUSE_FIRST || btn > __ST_MOUSE_LAST) {
        st_warn("Unknown mouse button: %d\n", btn);
        return false;
    }

    return m->states[btn].curr;
}

bool st_mouse_press(struct st_mouse *m, int btn)
{
    if (!m)
        return false;

    if (btn < __ST_MOUSE_FIRST || btn > __ST_MOUSE_LAST) {
        st_warn("Unknown mouse button: %d\n", btn);
        return false;
    }

    return m->states[btn].curr && !m->states[btn].prev;
}

bool st_mouse_release(struct st_mouse *m, int btn)
{
    if (!m)
        return false;

    if (btn < __ST_MOUSE_FIRST || btn > __ST_MOUSE_LAST) {
        st_warn("Unknown mouse button: %d\n", btn);
        return false;
    }

    return !m->states[btn].curr && m->states[btn].prev;
}
