#include <string.h>

#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>

#include <st/engine.h>
#include <st/print.h>
#include <st/vector.h>
#include <st/gfx/camera.h>
#include <st/gfx/renderer2d.h>

#include <st/util.h>

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

static void gl_init(StRenderer2d *renderer)
{
    glGenVertexArrays(1, &renderer->gl.vao);
    glBindVertexArray(renderer->gl.vao);

    glGenBuffers(1, &renderer->gl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, vector_capacity(renderer->vertex_buffer) * sizeof(StVertex2d),
        NULL, GL_DYNAMIC_DRAW);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(StVertex2d), (void *)offsetof(StVertex2d, position));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(StVertex2d), (void *)offsetof(StVertex2d, color));
    glEnableVertexAttribArray(1);

    const char *vertex_path = ST_ASSETS_PATH "/shaders/vertex_2d.glsl";
    const char *fragment_path = ST_ASSETS_PATH "/shaders/fragment_2d.glsl";
    renderer->gl.program = create_shader(vertex_path, fragment_path);
    assert(renderer->gl.program);

    st_debug("Created shader: (id=%d)\n", renderer->gl.program);
    st_debug("- vert: '%s'\n", vertex_path);
    st_debug("- frag: '%s'\n", fragment_path);
}

static void gl_destroy(StRenderer2d *renderer)
{
    glDeleteBuffers(1, &renderer->gl.vbo);
    glDeleteVertexArrays(1, &renderer->gl.vao);
    glDeleteProgram(renderer->gl.program);
}

static void gl_begin(StRenderer2d *renderer)
{
    int width, height;
    st_window_get_size(&width, &height);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderer->gl.program);

    glUniformMatrix4fv(glGetUniformLocation(renderer->gl.program, "u_view_mat"),
        1, GL_FALSE, *renderer->camera->view_mat);
    glUniformMatrix4fv(glGetUniformLocation(renderer->gl.program, "u_projection_mat"),
        1, GL_FALSE, *renderer->camera->proj_mat);
}

static void gl_flush(StRenderer2d *renderer)
{
    glBindVertexArray(renderer->gl.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    const int offset = renderer->vertex_pointer - renderer->vertex_buffer;
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(StVertex2d), renderer->vertex_buffer);

    glDrawArrays(GL_TRIANGLES, 0, offset);

    renderer->vertex_pointer = renderer->vertex_buffer;
}

void st_renderer2d_init(StRenderer2d *renderer, StCamera *camera)
{
    assert(renderer);
    assert(camera);

    memset(renderer, 0, sizeof(*renderer));
    renderer->camera = camera;

    vector_reserve(renderer->vertex_buffer, ST_VERTEX_BUFFER_SIZE);
    assert(renderer->vertex_buffer);
    renderer->vertex_pointer = renderer->vertex_buffer;

    st_debug("Vertex buffer capacity: %d vertices, %d bytes\n",
        vector_capacity(renderer->vertex_buffer), 
        vector_capacity(renderer->vertex_buffer) * sizeof(StVertex2d));

    gl_init(renderer);

    st_debug("2D renderer created\n");
}

void st_renderer2d_destroy(StRenderer2d *renderer)
{
    if (!renderer)
        return;

    vector_free(renderer->vertex_buffer);

    gl_destroy(renderer);

    st_debug("2D renderer destroyed\n");
}

void st_renderer2d_begin(StRenderer2d *renderer)
{
    assert(renderer);

    gl_begin(renderer);
}

void st_renderer2d_end(StRenderer2d *renderer)
{
    gl_flush(renderer);
}

void st_renderer2d_draw(StRenderer2d *renderer, StVertex2d *vertices, int count)
{
    assert(renderer);
    assert(vertices);
    assert(count > 0);

    StEngine *engine = st_engine_context();
    if (!engine) {
        st_error("No engine context\n");
        return;
    }

    if (!engine->window) {
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

void st_renderer2d_draw_quad(StRenderer2d *renderer, vec3 position,
    vec3 rotation, vec3 scale, vec4 color)
{
    StVertex2d vertices[] = {
        {{1.0f, 1.0f, 0.0f}, {color[0], color[1], color[2], color[3]}},
        {{1.0f, 0.0f, 0.0f}, {color[0], color[1], color[2], color[3]}},
        {{0.0f, 1.0f, 0.0f}, {color[0], color[1], color[2], color[3]}},

        {{1.0f, 0.0f, 0.0f}, {color[0], color[1], color[2], color[3]}},
        {{0.0f, 0.0f, 0.0f}, {color[0], color[1], color[2], color[3]}},
        {{0.0f, 1.0f, 0.0f}, {color[0], color[1], color[2], color[3]}}
    };
    
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, position);
    glm_scale(model, scale);
    glm_rotate(model, glm_rad(rotation[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[2]), (vec3){0.0f, 0.0f, 1.0f});

    for (int i = 0; i < 6; i++) {
        glm_mat4_mulv3(model, vertices[i].position, 1.0f, vertices[i].position);
    }

    st_renderer2d_draw(renderer, vertices, 6);
} 
