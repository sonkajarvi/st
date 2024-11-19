#include <string.h>

#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>

#include <st/instance.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/utility/print.h>
#include <st/utility/util.h>
#include <st/utility/vector.h>

// #define __ST_FONT_PATH ST_ASSETS_PATH "/images/fonts/vga.png"
#define __ST_FONT_PATH ST_ASSETS_PATH "/images/fonts/nes.png"

static int __index_next_available(void)
{
    static int cnt = 0;
    return cnt++;
}

void st_renderer_init(StRenderer *renderer, StCamera *camera)
{
    st_assert(renderer);
    st_assert(camera);

    memset(renderer, 0, sizeof(*renderer));
    renderer->camera = camera;

    st_vector_reserve(renderer->vertex_buf, ST_VERTEX_BUFFER_SIZE);
    st_assert(renderer->vertex_buf);
    renderer->vertex_ptr = renderer->vertex_buf;

    st_debug("Vertex buffer created for %zu vertices, %zu bytes\n",
        st_vector_capacity(renderer->vertex_buf),
        st_vector_capacity(renderer->vertex_buf) * sizeof(StVertex));

    // White 1x1 texture
    struct st_image tmp = {
        .width = 1,
        .height = 1,
        .data = (uint8_t[]){ 0xff, 0xff, 0xff, 0xff },
    };
    st_texture_create(&renderer->tex_white, &tmp);

    // Font texture
    st_image_from_file(&tmp, __ST_FONT_PATH);
    st_texture_create(&renderer->tex_font, &tmp);

    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    call_impl(st, renderer_init, window, renderer, camera);

    st_debug("2D renderer created\n");
}

void st_renderer_destroy(StRenderer *renderer)
{
    st_assert(renderer);

    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    call_impl(st, renderer_destroy, window, renderer);

    st_vector_free(renderer->vertex_buf);

    st_debug("2D renderer destroyed\n");
}

void st_draw_begin(struct st_window *window)
{
    st_assert(window);

    struct st *st = st_instance();
    st_assert(st);
    StRenderer *renderer = &window->renderer;

    call_impl(st, draw_begin, window, renderer);
}

void st_draw_end(struct st_window *window)
{
    st_assert(window);

    struct st *st = st_instance();
    st_assert(st);
    StRenderer *renderer = &window->renderer;

    call_impl(st, draw_end, window, renderer);
}

void st_renderer_push(StRenderer *renderer, StVertex *vertices, size_t count)
{
    st_assert(renderer);
    st_assert(vertices);
    st_assert(count > 0);

    struct st *st = st_instance();
    st_assert(st);
    struct st_window *window = st->window;
    st_assert(window);

    call_impl(st, renderer_push, window, renderer, vertices, count);
}

void st_draw_quad(struct st_window *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color)
{
    st_draw_textured_quad(window, position, rotation, scale, color,
        &window->renderer.tex_white, (vec4){0.0f, 0.0f, 1.0f, 1.0f});
}

void st_draw_textured_quad(struct st_window *window,
    vec3 position, vec3 rotation, vec3 scale, vec4 color,
    struct st_texture *tex, vec4 tex_coords)
{
    if (!window || !tex)
        return;

    if (tex->index == -1) {
        tex->index = __index_next_available();
        window->renderer.gl.tex_ids[tex->index] = tex->gl.id;
    }

    const float index = (float)tex->index;

    const float t0 = tex_coords[0] / tex->width;
    const float t1 = tex_coords[1] / tex->height;
    const float t2 = tex_coords[2] / tex->width + t0;
    const float t3 = tex_coords[3] / tex->height + t1;

// todo: there has to be a better way
#define __x(c) c[0], c[1], c[2], c[3]

    StVertex vertices[] = {
        {{ 1.0f,  1.0f, 0.0f}, {__x(color)}, {t2, t3}, index}, // top right
        {{ 1.0f, -1.0f, 0.0f}, {__x(color)}, {t2, t1}, index}, // bottom right
        {{-1.0f,  1.0f, 0.0f}, {__x(color)}, {t0, t3}, index}, // top left

        {{ 1.0f, -1.0f, 0.0f}, {__x(color)}, {t2, t1}, index}, // bottom right
        {{-1.0f, -1.0f, 0.0f}, {__x(color)}, {t0, t1}, index}, // bottom left
        {{-1.0f,  1.0f, 0.0f}, {__x(color)}, {t0, t3}, index}  // top left
    };

#undef __X

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, position);
    glm_rotate(model, glm_rad(rotation[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(model, scale);

    for (int i = 0; i < 6; i++) {
        glm_mat4_mulv3(model, vertices[i].position, 1.0f, vertices[i].position);
    }

    st_renderer_push(&window->renderer, vertices, 6);
}

void st_draw_line(struct st_window *window, vec3 p0, vec3 p1, vec4 color, float width)
{
    const float half = width / 2.0f;
    const vec2 a = { p1[0] - p0[0], p1[1] - p0[1] };
    vec3 b = { a[1], -a[0], 0.0f };
    glm_normalize(b);

    const vec3 p2 = {p0[0] + b[0] * half, p0[1] + b[1] * half, p0[2]};
    const vec3 p3 = {p0[0] - b[0] * half, p0[1] - b[1] * half, p0[2]};
    const vec3 p4 = {p1[0] + b[0] * half, p1[1] + b[1] * half, p1[2]};
    const vec3 p5 = {p1[0] - b[0] * half, p1[1] - b[1] * half, p1[2]};

    StVertex vertices[] = {
        {{p2[0], p2[1], p2[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f},
        {{p3[0], p3[1], p3[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f},
        {{p4[0], p4[1], p4[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f},

        {{p3[0], p3[1], p3[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f},
        {{p4[0], p4[1], p4[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f},
        {{p5[0], p5[1], p5[2]}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, 0.0f}
    };

    st_renderer_push(&window->renderer, vertices, 6);
}
