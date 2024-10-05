#ifndef ST_GFX_RENDERER2D_H
#define ST_GFX_RENDERER2D_H

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>

#include <st/gfx/camera.h>

#define ST_VERTEX_BUFFER_SIZE (256 * 3)

typedef struct StVertex2d
{
    vec3 position;
    vec4 color;
} StVertex2d;

typedef struct StRenderer2d
{
    StVertex2d *vertex_buffer;
    StVertex2d *vertex_pointer;
 
    struct {
        GLuint program;
        GLuint vao;
        GLuint vbo;
    } gl;
    StCamera *camera;
} StRenderer2d;

void st_renderer2d_init(StRenderer2d *renderer, StCamera *camera);
void st_renderer2d_destroy(StRenderer2d *renderer);

void st_renderer2d_begin(StRenderer2d *renderer);
void st_renderer2d_end(StRenderer2d *renderer);
void st_renderer2d_draw(StRenderer2d *renderer, StVertex2d *vertices, int count);

void st_renderer2d_draw_quad(StRenderer2d *renderer, vec3 position,
    vec3 rotation, vec3 scale, vec4 color);

#endif // ST_GFX_RENDERER2D_H
