#ifndef ST_GRAPHICS_DRAW_TEXT_H
#define ST_GRAPHICS_DRAW_TEXT_H

#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include <st/window.h>

// Font character map
// ==================

// * / % = # @ & _ $ | \ ^ ` ~
// . , : ; ' " ( ) [ ] { } < > + -
// W X Y Z 0 1 2 3 4 5 6 7 8 9 ! ?
// G H I J K L M N O P Q R S T U V
// q r s t u v w x y z A B C D E F
// a b c d e f g h i j k l m n o p

#define _ST_CHARS_5 "*/%=#@&_$|\\^`~"
#define _ST_CHARS_4 ".,:;'\"()[]{}<>+-"
#define _ST_CHARS_3 "WXYZ0123456789!?"
#define _ST_CHARS_2 "GHIJKLMNOPQRSTUV"
#define _ST_CHARS_1 "qrstuvwxyzABCDEF"
#define _ST_CHARS_0 "abcdefghijklmnop"

void st_draw_char(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char c);

void st_draw_string(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char *str);

#endif // ST_GRAPHICS_DRAW_TEXT_H
