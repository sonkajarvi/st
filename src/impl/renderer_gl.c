#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <st/utility/assert.h>
#include <st/utility/print.h>
#include <st/utility/util.h>
#include <st/utility/vector.h>

#include "renderer_gl.h"
#include "st/graphics/renderer.h"

#define _SHADER_LOG_BUFSZ 512

static GLuint __create_shader(const char *vertex_path, const char *fragment_path)
{
    char log[_SHADER_LOG_BUFSZ];
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
        glGetShaderInfoLog(vertex_shader, _SHADER_LOG_BUFSZ, NULL, log);
        st_error("Vertex shader error:\n%s\n", log);
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
        glGetShaderInfoLog(fragment_shader, _SHADER_LOG_BUFSZ, NULL, log);
        st_error("Fragment shader error:\n%s\n", log);
        goto end;
    }

    // Shader program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 256, NULL, log);
        st_error("Failed to link program:\n%s\n", log);
    }

end:
    free((char *)fragment_source);
end2:
    free((char *)vertex_source);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}

static void __flush(StRenderer *renderer)
{
    glBindVertexArray(renderer->gl.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    const int offset = renderer->vertex_ptr - renderer->vertex_buf;
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(StVertex), renderer->vertex_buf);
    glDrawArrays(GL_TRIANGLES, 0, offset);

    renderer->vertex_ptr = renderer->vertex_buf;
}

void impl_gl_renderer_init(StWindow *window, StRenderer *renderer, StCamera *camera)
{
    st_assert(window);
    st_assert(renderer);
    st_assert(camera);

    glGenVertexArrays(1, &renderer->gl.vao);
    glBindVertexArray(renderer->gl.vao);

    glGenBuffers(1, &renderer->gl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, vector_capacity(renderer->vertex_buf) * sizeof(StVertex),
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
    renderer->gl.program = __create_shader(vertex_path, fragment_path);
    st_assert(renderer->gl.program);

    st_debug("Shader created\n");
    st_debug("... id: %d\n", renderer->gl.program);
    st_debug("... vertex: '%s'\n", vertex_path);
    st_debug("... fragment: '%s'\n", fragment_path);
}

void impl_gl_draw_begin(StWindow *window, StRenderer *renderer)
{
    st_assert(window);
    st_assert(renderer);

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

void impl_gl_draw_end(StWindow *window, StRenderer *renderer)
{
    assert(window);
    assert(renderer);

    __flush(renderer);
}

void impl_gl_renderer_destroy(StWindow *window, StRenderer *renderer)
{
    st_assert(window);
    st_assert(renderer);

    glDeleteBuffers(1, &renderer->gl.vbo);
    glDeleteVertexArrays(1, &renderer->gl.vao);
    glDeleteProgram(renderer->gl.program);

    vector_for(renderer->textures, StTexture *, texture)
        glDeleteTextures(1, &(*texture)->gl.id);
}

void impl_gl_renderer_add_texture(
    StWindow *window, StRenderer *renderer, StTexture *texture)
{
    st_assert(window);
    st_assert(renderer);
    st_assert(texture);

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

void impl_gl_renderer_push(StWindow * window,
    StRenderer * renderer, StVertex *vertices, size_t count)
{
    st_assert(window);
    st_assert(renderer);
    st_assert(vertices);
    st_assert(count > 0);

    const size_t offset = renderer->vertex_ptr - renderer->vertex_buf;
    if (offset + count > ST_VERTEX_BUFFER_SIZE)
        __flush(renderer);

    for (size_t i = 0; i < count; i++)
        *renderer->vertex_ptr++ = vertices[i];
}