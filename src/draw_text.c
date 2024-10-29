#include <cglm/vec4.h>

#include <st/graphics/draw_text.h>
#include <st/graphics/renderer.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>

static float __char_to_x(const char c)
{
    switch (c) {
    case '*':
    case '.':
    case 'W':
    case 'G':
    case 'q':
    case 'a':
        return 0.0f;

    case '/':
    case ',':
    case 'X':
    case 'H':
    case 'r':
    case 'b':
        return 16.0f;

    case '%':
    case ':':
    case 'Y':
    case 'I':
    case 's':
    case 'c':
        return 32.0f;

    case '=':
    case ';':
    case 'Z':
    case 'J':
    case 't':
    case 'd':
        return 48.0f;

    case '#':
    case '\'':
    case '0':
    case 'K':
    case 'u':
    case 'e':
        return 64.0f;

    case '@':
    case '"':
    case '1':
    case 'L':
    case 'v':
    case 'f':
        return 80.0f;

    case '&':
    case '(':
    case '2':
    case 'M':
    case 'w':
    case 'g':
        return 96.0f;

    case '_':
    case ')':
    case '3':
    case 'N':
    case 'x':
    case 'h':
        return 112.0f;

    case '$':
    case '[':
    case '4':
    case 'O':
    case 'y':
    case 'i':
        return 128.0f;

    case '|':
    case ']':
    case '5':
    case 'P':
    case 'z':
    case 'j':
        return 144.0f;

    case '\\':
    case '{':
    case '6':
    case 'Q':
    case 'A':
    case 'k':
        return 160.0f;

    case '^':
    case '}':
    case '7':
    case 'R':
    case 'B':
    case 'l':
        return 176.0f;

    case '`':
    case '<':
    case '8':
    case 'S':
    case 'C':
    case 'm':
        return 192.0f;

    case '~':
    case '>':
    case '9':
    case 'T':
    case 'D':
    case 'n':
        return 208.0f;

    case '+':
    case '!':
    case 'U':
    case 'E':
    case 'o':
        return 224.0f;

    case '-':
    case '?':
    case 'V':
    case 'F':
    case 'p':
        return 240.0f;
    }

    return -0.0f;
}

static float __char_to_y(const char c)
{
    switch (c) {
    case 'a' ... 'p':
        return 0.0f;

    case 'q' ... 'z':
    case 'A' ... 'F':
        return 16.0f;

    case 'G' ... 'V':
        return 32.0f;

    case 'W' ... 'Z':
    case '0' ... '9':
    case '!':
    case '?':
        return 48.0f;

    case '.':
    case ',':
    case ':':
    case ';':
    case '\'':
    case '"':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '<':
    case '>':
    case '+':
    case '-':
        return 64.0f;

    case '*':
    case '/':
    case '%':
    case '=':
    case '#':
    case '@':
    case '&':
    case '_':
    case '|':
    case '\\':
    case '^':
    case '`':
    case '~':
        return 80.0f;
    }

    return -0.0f;
}

void st_draw_char(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, const char c)
{
    st_assert(window);

    vec4 tex_coords = { __char_to_x(c), __char_to_y(c), 16.0f, 16.0f };
    StTexture *font = window->renderer.textures[1];

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
        if (c == ' ')
            goto skip;

        st_draw_char(window, position, rotation, scale, color, c);
skip:
        position[0] += scale[0];
    }
}
