#ifndef ST_GRAPHICS_RENDERER2D_H
#define ST_GRAPHICS_RENDERER2D_H

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>

#include <st/graphics/camera.h>
#include <st/graphics/texture.h>

typedef struct StWindow StWindow;

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
    StCamera *camera;
    StTexture **textures;
    StVertex *vertex_buffer;
    StVertex *vertex_pointer;

    struct {
        GLuint program;
        GLuint vao;
        GLuint vbo;
    } gl;
} StRenderer;

void st_renderer_init(StRenderer *renderer, StCamera *camera);
void st_renderer_destroy(StRenderer *renderer);

void st_renderer_add_texture(StRenderer *renderer, StTexture *texture);
void st_renderer_push(StRenderer *renderer, StVertex *vertices, int count);

void st_draw_begin(StWindow *window);
void st_draw_end(StWindow *window);

void st_draw_quad(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color);
void st_draw_textured_quad(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, StTexture *texture);

void st_draw_line(StWindow *window, vec3 p0, vec3 p1, vec4 color, float width);

#endif // ST_GRAPHICS_RENDERER2D_H
