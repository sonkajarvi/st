#include <cglm/vec4.h>

#include <st/graphics/draw_text.h>
#include <st/graphics/renderer.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>

void st_draw_char(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char c)
{
    st_assert(window);

    StTexture *font = window->renderer.textures[1];

    const char n = c - 32;
    float x = 16.0f * (n % 16);
    float y = font->height - (16.0f * (n / 16)) - 16.0f;

    vec4 tex_coords = { x, y, 16.0f, 16.0f };

    st_draw_textured_quad(window,
        position, rotation, scale, color, font, tex_coords);
}

void st_draw_string(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char *str)
{
    st_assert(window);
    st_assert(str);

    char c;
    while ((c = *str++)) {
        if (c != ' ')
            st_draw_char(window, position, rotation, scale, color, c);

        position[0] += scale[0];
    }
}
