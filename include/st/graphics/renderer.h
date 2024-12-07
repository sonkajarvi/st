#ifndef ST_GRAPHICS_RENDERER2D_H
#define ST_GRAPHICS_RENDERER2D_H

#include <cglm/types.h>
#include <glad/gl.h>

#include <st/graphics/camera.h>
#include <st/graphics/texture.h>

struct st_window;

#define ST_VERTEX_BUFFER_SIZE (256 * 3)

typedef struct StVertex
{
    vec3 position;
    vec4 color;
    vec2 tex_coords;
    float tex_index;
} StVertex;

typedef struct StRenderer
{
    struct st_camera *camera;
    StVertex *vertex_buf;
    StVertex *vertex_ptr;

    struct st_texture tex_white;
    struct st_texture tex_font;

    struct {
        GLuint program;
        GLuint vao;
        GLuint vbo;
        GLuint tex_ids[4];
    } gl;
} StRenderer;

void st_renderer_init(StRenderer *renderer, struct st_camera *camera);
void st_renderer_destroy(StRenderer *renderer);

void st_renderer_push(StRenderer *renderer, StVertex *vertices, size_t count);

void st_draw_begin(struct st_window *window);
void st_draw_end(struct st_window *window);

void st_draw_quad(struct st_window *window,
    vec3 position, vec3 rotation, vec3 scale, vec4 color);

void st_draw_textured_quad(struct st_window *window,
    vec3 position, vec3 rotation, vec3 scale, vec4 color,
    struct st_texture *tex, vec4 tex_coords);

void st_draw_line(struct st_window *window, vec3 p0, vec3 p1, vec4 color,
    float width);

void st_draw_quad_outline(struct st_window *window, vec3 position, vec3 scale,
    vec4 color, float width);

void st_draw_char(struct st_window *window, vec3 position, vec3 rotation,
    vec3 scale, vec4 color, const char c);

void st_draw_string(struct st_window *window, vec3 position, vec3 rotation,
    vec3 scale, vec4 color, const char *str);

#endif // ST_GRAPHICS_RENDERER2D_H
