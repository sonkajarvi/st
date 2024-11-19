#include <st/instance.h>
#include <st/window.h>
#include <st/input/mouse.h>
#include <st/utility/print.h>

// void mouse_get_pos(int *x, int *y)
// {
//     struct st *st = st_instance();
//     st_assert(st);
//     struct st_window *window = st->window;
//     st_assert(window);

//     if (x)
//         *x = window->mouse.position[0];
//     if (y)
//         *y = window->mouse.position[1];
// }

// void mouse_get_delta(int *x, int *y)
// {
//     struct st *st = st_instance();
//     st_assert(st);
//     struct st_window *window = st->window;
//     st_assert(window);

//     if (x)
//         *x = window->mouse.delta[0];
//     if (y)
//         *y = window->mouse.delta[1];
// }

// float st_mouse_get_wheel(void)
// {
//     // struct st *st = st_instance();
//     // st_assert(st);
//     // struct st_window *window = st->window;
//     // st_assert(window);

//     return window->mouse.wheel;
// }

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
