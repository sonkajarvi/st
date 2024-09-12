#ifndef ST_INPUT_KEYBOARD_H
#define ST_INPUT_KEYBOARD_H

#include <st/input/keys.h>

typedef struct StKeyboard
{
    struct {
        char current : 1;
        char previous : 1;
    } state[__ST_KEY_COUNT];
} StKeyboard;

#endif // ST_INPUT_KEYBOARD_H
