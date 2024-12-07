#include <st/window.h>
#include <st/graphics/renderer.h>
#include <st/util/assert.h>
#include <st/util/print.h>

void st_draw_char(struct st_window *window, vec3 position, vec3 rotation,
    vec3 scale, vec4 color, const char c)
{
    st_assert(window);

    struct st_texture *font = &window->renderer.tex_font;

    const char n = c - 32;
    float x = 16.0f * (n % 16);
    float y = font->height - (16.0f * (n / 16)) - 16.0f;

    vec4 tex_coords = { x, y, 16.0f, 16.0f };

    st_draw_textured_quad(window,
        position, rotation, scale, color, font, tex_coords);
}

void st_draw_string(struct st_window *window, vec3 position, vec3 rotation,
    vec3 scale, vec4 color, const char *str)
{
    st_assert(window);
    st_assert(str);

    const float origin_x = position[0];

    char c;
    while ((c = *str++)) {
        if (c == '\n') {
            position[0] = origin_x;
            position[1] -= scale[1];
            continue;
        }

        if (c != ' ')
            st_draw_char(window, position, rotation, scale, color, c);

        if (c == '\t')
            position[0] += scale[0] * 3.0f;

        position[0] += scale[0] / 2.0f;
    }
}
