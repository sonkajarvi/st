#ifndef ST_GRAPHICS_DRAW_TEXT_H
#define ST_GRAPHICS_DRAW_TEXT_H

#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include <st/window.h>

void st_draw_char(struct st_window *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char c);

void st_draw_string(struct st_window *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char *str);

#endif // ST_GRAPHICS_DRAW_TEXT_H
