#ifndef ST_INPUT_KEYBOARD_H
#define ST_INPUT_KEYBOARD_H

#include <stdbool.h>

#include <st/input/keys.h>

typedef struct StKeyboard
{
    struct {
        char current : 1;
        char previous : 1;
    } state[__ST_KEY_COUNT];
} StKeyboard;

bool key_down(int key);
bool key_press(int key);
bool key_release(int key);

#endif // ST_INPUT_KEYBOARD_H
