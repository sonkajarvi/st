#ifndef ST_INPUT_MOUSE_H
#define ST_INPUT_MOUSE_H

#include <cglm/ivec2.h>

#define ST_MOUSE_LEFT   0
#define ST_MOUSE_RIGHT  1
#define ST_MOUSE_MIDDLE 2
#define ST_MOUSE_X1     3
#define ST_MOUSE_X2     4

#define __ST_MOUSE_FIRST    ST_MOUSE_LEFT
#define __ST_MOUSE_LAST     ST_MOUSE_X2
#define __ST_MOUSE_COUNT    (__ST_MOUSE_LAST + 1)

typedef struct StMouse
{
    ivec2 position;
    ivec2 delta;
    float wheel;
    struct {
        char current : 1;
        char previous : 1;
    } state[__ST_MOUSE_COUNT];
} StMouse;

#endif // ST_INPUT_MOUSE_H
