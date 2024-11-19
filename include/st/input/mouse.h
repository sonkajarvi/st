#ifndef ST_INPUT_MOUSE_H
#define ST_INPUT_MOUSE_H

#include <stdbool.h>

#define ST_MOUSE_LEFT   0
#define ST_MOUSE_RIGHT  1
#define ST_MOUSE_MIDDLE 2
#define ST_MOUSE_X1     3
#define ST_MOUSE_X2     4

#define __ST_MOUSE_FIRST    ST_MOUSE_LEFT
#define __ST_MOUSE_LAST     ST_MOUSE_X2

struct st_mouse
{
    struct {
        bool prev, curr;
    } states[5];
    int x, y, dx, dy;
    float wheel;
};

// st_mouse_down() - Check if a mouse button is being held down
// @btn: Mouse button to check
bool st_mouse_down(struct st_mouse *m, int btn);

// st_mouse_press() - Check if a mouse button was pressed
// @btn: Mouse button to check
bool st_mouse_press(struct st_mouse *m, int btn);

// st_mouse_release() - Check if a mouse button was released
// @btn: Mouse button to check
bool st_mouse_release(struct st_mouse *m, int btn);

// void mouse_get_pos(int *x, int *y);
// void mouse_get_delta(int *x, int *y);
// float mouse_get_wheel(void);

#endif // ST_INPUT_MOUSE_H
