#include <string.h>

#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <st/instance.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer.h>
#include <st/utility/print.h>
#include <st/utility/util.h>
#include <st/utility/vector.h>

static GLuint create_shader(const char *vertex_path, const char *fragment_path)
{
    char output[256];
    int success;
    GLuint program = 0, vertex_shader = 0, fragment_shader = 0;

    // Vertex shader
    const char *vertex_source = read_file(vertex_path);
    if (!vertex_source) {
        st_error("Failed to read '%s'\n", vertex_path);
        goto end2;
    }

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 256, NULL, output);
        st_error("Vertex shader error:\n%s\n", output);
        goto end2;
    }

    // Fragment shader
    const char *fragment_source = read_file(fragment_path);
    if (!fragment_source) {
        st_error("Failed to read '%s'\n", fragment_path);
        goto end;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 256, NULL, output);
        st_error("Fragment shader error:\n%s\n", output);
        goto end;
    }

    // Shader program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 256, NULL, output);
        st_error("Failed to link program:\n%s\n", output);
    }

end:
    free((char *)fragment_source);
end2:
    free((char *)vertex_source);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}

static void gl_init(StRenderer *renderer)
{
    glGenVertexArrays(1, &renderer->gl.vao);
    glBindVertexArray(renderer->gl.vao);

    glGenBuffers(1, &renderer->gl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, vector_capacity(renderer->vertex_buffer) * sizeof(StVertex),
        NULL, GL_DYNAMIC_DRAW);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(StVertex), (void *)offsetof(StVertex, position));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(StVertex), (void *)offsetof(StVertex, color));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(StVertex), (void *)offsetof(StVertex, tex_coords));
    glEnableVertexAttribArray(2);

    // Texture index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
        sizeof(StVertex), (void *)offsetof(StVertex, tex_index));
    glEnableVertexAttribArray(3);

    const char *vertex_path = ST_ASSETS_PATH "/shaders/vertex_2d.glsl";
    const char *fragment_path = ST_ASSETS_PATH "/shaders/fragment_2d.glsl";
    renderer->gl.program = create_shader(vertex_path, fragment_path);
    st_assert(renderer->gl.program);

    st_debug("Shader created\n");
    st_debug("... id: %d\n", renderer->gl.program);
    st_debug("... vertex: '%s'\n", vertex_path);
    st_debug("... fragment: '%s'\n", fragment_path);
}

static void gl_destroy(StRenderer *renderer)
{
    glDeleteBuffers(1, &renderer->gl.vbo);
    glDeleteVertexArrays(1, &renderer->gl.vao);
    glDeleteProgram(renderer->gl.program);

    vector_for(renderer->textures, StTexture *, texture)
        glDeleteTextures(1, &(*texture)->gl.id);
}

static void gl_begin(StRenderer *renderer)
{
    int width, height;
    st_window_get_size(&width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderer->gl.program);

    const GLint array[] = {0, 1, 2, 3};
    glUniform1iv(glGetUniformLocation(renderer->gl.program, "u_textures"),
        4, array);

    for (size_t i = 0; i < vector_length(renderer->textures); i++)
        glBindTextureUnit(i, renderer->textures[i]->gl.id);

    glUniformMatrix4fv(glGetUniformLocation(renderer->gl.program, "u_view_mat"),
        1, GL_FALSE, *renderer->camera->view_mat);
    glUniformMatrix4fv(glGetUniformLocation(renderer->gl.program, "u_projection_mat"),
        1, GL_FALSE, *renderer->camera->proj_mat);
}

static void gl_flush(StRenderer *renderer)
{
    glBindVertexArray(renderer->gl.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    const int offset = renderer->vertex_pointer - renderer->vertex_buffer;
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(StVertex), renderer->vertex_buffer);

    glDrawArrays(GL_TRIANGLES, 0, offset);

    renderer->vertex_pointer = renderer->vertex_buffer;
}

static void gl_init_texture(StTexture *texture)
{
    glGenTextures(1, &texture->gl.id);
    glBindTexture(GL_TEXTURE_2D, texture->gl.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unsigned char *data;
    if (texture->from_file) {
        int channels;

        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(texture->data.path,
            &texture->width, &texture->height, &channels, 4);
        st_assert(data);
    } else {
        data = texture->data.bytes;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width,
        texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (texture->from_file)
        stbi_image_free(data);
}

void st_renderer_init(StRenderer *renderer, StCamera *camera)
{
    st_assert(renderer);
    st_assert(camera);

    memset(renderer, 0, sizeof(*renderer));
    renderer->camera = camera;

    vector_reserve(renderer->vertex_buffer, ST_VERTEX_BUFFER_SIZE);
    st_assert(renderer->vertex_buffer);
    renderer->vertex_pointer = renderer->vertex_buffer;

    st_debug("Vertex buffer created (capacity: %d vertices, %d bytes)\n",
        vector_capacity(renderer->vertex_buffer), 
        vector_capacity(renderer->vertex_buffer) * sizeof(StVertex));

    gl_init(renderer);

    // 1x1 white texture
    static StTexture white = {0};
    unsigned char bytes[] = {0xff, 0xff, 0xff, 0xff};
    st_texture_from_bytes(&white, bytes, 1, 1);
    st_renderer_add_texture(renderer, &white);

    // Font texture
    static StTexture font = {0};
    st_texture_from_file(&font, "assets/images/font.png");
    st_renderer_add_texture(renderer, &font);

    st_debug("2D renderer created\n");
}

void st_renderer_destroy(StRenderer *renderer)
{
    if (!renderer)
        return;

    vector_free(renderer->vertex_buffer);

    gl_destroy(renderer);

    vector_for(renderer->textures, StTexture *, texture)
        st_texture_destroy(*texture);

    st_debug("2D renderer destroyed\n");
}

void st_renderer_add_texture(StRenderer *renderer, StTexture *texture)
{
    st_assert(renderer);
    st_assert(texture);
    st_assert(vector_length(renderer->textures) <= 4);

    vector_push(renderer->textures, texture);

    gl_init_texture(texture);

    st_debug("Texture added to 2D renderer\n");
    st_debug("... id: %d\n", texture->gl.id);
    st_debug("... size: %dx%d\n", texture->width, texture->height);
}

void st_draw_begin(StWindow *window)
{
    st_assert(window);

    gl_begin(&window->renderer);
}

void st_draw_end(StWindow *window)
{
    gl_flush(&window->renderer);
}

void st_renderer_push(StRenderer *renderer, StVertex *vertices, int count)
{
    st_assert(renderer);
    st_assert(vertices);
    st_assert(count > 0);

    St *st = st_instance();
    if (!st) {
        st_error("No engine context\n");
        return;
    }

    if (!st->window) {
        st_error("No window found\n");
        return;
    }

    const int offset = renderer->vertex_pointer - renderer->vertex_buffer;
    if (offset + count > ST_VERTEX_BUFFER_SIZE)
        gl_flush(renderer);

    for (int i = 0; i < count; i++) {
        *renderer->vertex_pointer++ = vertices[i];
    }
}

void st_draw_quad(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color)
{
    st_draw_textured_quad(window, position, rotation, scale, color,
        window->renderer.textures[0], (vec4){0.0f, 0.0f, 1.0f, 1.0f});
}

static int index_from_id(StRenderer *renderer, GLuint id)
{
    for (size_t i = 0; i < vector_length(renderer->textures); i++) {
        if (renderer->textures[i]->gl.id == id)
            return (int)i;
    }
    return -1;
}

void st_draw_textured_quad(StWindow *window, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, StTexture *texture, vec4 tex_coords)
{
    const float index = (float)index_from_id(&window->renderer, texture->gl.id);

    const float t0 = tex_coords[0] / texture->width;
    const float t1 = tex_coords[1] / texture->height;
    const float t2 = tex_coords[2] / texture->width + t0;
    const float t3 = tex_coords[3] / texture->height + t1;

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

void st_draw_line(StWindow *window, vec3 p0, vec3 p1, vec4 color, float width)
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
