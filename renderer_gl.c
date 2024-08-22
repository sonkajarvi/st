#include "renderer_gl.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <glad/glad_glx.h>
#include <cglm/mat4.h>
#include <cglm/cam.h>
#include <cglm/affine.h>

#include "cglm/vec3.h"
#include "common.h"
#include "util.h"

#define INDEX_COUNT 256

struct gl_renderer
{
    struct st_vertex vertex_buffer[3 * INDEX_COUNT];
    struct st_vertex *vertex_buffer_ptr;
    unsigned int indices[INDEX_COUNT];

    struct st_camera *camera;

    GLuint shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    // per draw call
    size_t draw_count;
    size_t index_count;
    size_t vertex_count;
    size_t mesh_count;
    
    // from begin to end
    size_t total_index_count;
    size_t total_vertex_count;
    size_t total_mesh_count;

    bool first_draw;
};

static struct gl_renderer renderer;

static GLuint shader_create(const char *vert_path, const char *frag_path)
{
    GLuint program = 0, vert_shader = 0, frag_shader = 0;
    int success;
    static char log[256];

    // vertex shader
    const char *vert_src = read_file(vert_path);
    assert(vert_src);

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_src, NULL);
    glCompileShader(vert_shader);
    free((char *)vert_src);

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert_shader, 256, NULL, log);
        fprintf(stderr, "vertex shader error:\n%s\n", log);
        goto end;
    }

    // fragment shader
    const char *frag_src = read_file(frag_path);
    assert(frag_src);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glCompileShader(frag_shader);
    free((char *)frag_src);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 256, NULL, log);
        fprintf(stderr, "fragment shader error:\n%s\n", log);
        goto end;
    }

    // shader program
    program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 256, NULL, log);
        fprintf(stderr, "shader linking error:\n%s\n", log);
    }

end:
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return program;
}

static void flush(void)
{
    int width, height;
    window_get_size(&width, &height);
    glViewport(0, 0, width, height);

    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_BLEND);

    // we may have multiple draw calls,
    // clear only on the first
    if (renderer.first_draw) {
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.first_draw = false;
    }

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    vec3 axis = {1.0f, 1.0f, 1.0f};
    static float angle = -55.0f;
    glm_rotate(model, glm_rad(angle), axis);
    angle--;
    
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, renderer.camera->position);

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    const float ratio = (float)width / (float)height;
    glm_perspective(glm_rad(45.0f), ratio, 0.1f, 100.0f, projection);

    glUniformMatrix4fv(glGetUniformLocation(renderer.shader, "u_View"),
        1, GL_FALSE, *view);
    glUniformMatrix4fv(glGetUniformLocation(renderer.shader, "u_Model"),
        1, GL_FALSE, *model);
    glUniformMatrix4fv(glGetUniformLocation(renderer.shader, "u_Projection"),
        1, GL_FALSE, *projection);

    size_t len = renderer.vertex_buffer_ptr - renderer.vertex_buffer;

    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, len * sizeof(struct st_vertex), renderer.vertex_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        renderer.index_count * sizeof(unsigned int), renderer.indices, GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, renderer.index_count, GL_UNSIGNED_INT, 0);
    renderer.draw_count++;

    renderer.total_index_count += renderer.index_count;
    renderer.total_vertex_count += renderer.vertex_count;
    renderer.total_mesh_count += renderer.mesh_count;

    renderer.index_count = 0;
    renderer.vertex_count = 0;
    renderer.mesh_count = 0;

    renderer.vertex_buffer_ptr = renderer.vertex_buffer;
}

void impl_gl_renderer_init(struct st_camera *camera)
{
    memset(&renderer, 0, sizeof(renderer));
    renderer.vertex_buffer_ptr = renderer.vertex_buffer;
    renderer.camera = camera;

    glGenVertexArrays(1, &renderer.vao);
    glBindVertexArray(renderer.vao);

    glGenBuffers(1, &renderer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(renderer.vertex_buffer), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer.ebo);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(struct st_vertex), (void *)offsetof(struct st_vertex, position));
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(struct st_vertex), (void *)offsetof(struct st_vertex, color));
    glEnableVertexAttribArray(1);

    renderer.shader = shader_create("assets/basic.vert", "assets/basic.frag");
    glUseProgram(renderer.shader);
}

void impl_gl_renderer_destroy(void)
{
    glDeleteBuffers(1, &renderer.vbo);
    glDeleteBuffers(1, &renderer.ebo);
    glDeleteVertexArrays(1, &renderer.vao);

    glDeleteProgram(renderer.shader);
}

void impl_gl_renderer_begin(void)
{
    renderer.vertex_buffer_ptr = renderer.vertex_buffer;

    renderer.draw_count = 0;
    renderer.index_count = 0;
    renderer.vertex_count = 0;
    renderer.mesh_count = 0;

    renderer.total_index_count = 0;
    renderer.total_vertex_count = 0;
    renderer.total_mesh_count = 0;

    renderer.first_draw = true;
}

void impl_gl_renderer_end(void)
{
    flush();
}

void impl_gl_renderer_push_mesh(const struct st_vertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count)
{
    if (renderer.index_count + index_count >= INDEX_COUNT)
        flush();

    for (size_t i = 0; i < vertex_count; i++) {
        *renderer.vertex_buffer_ptr++ = vertices[i];
    }

    for (size_t i = 0; i < index_count; i++) {
        renderer.indices[renderer.index_count++] = renderer.vertex_count + indices[i];
    }

    renderer.vertex_count += vertex_count;
    renderer.mesh_count++;
}
