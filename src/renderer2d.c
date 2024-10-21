#include <string.h>

#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <st/engine.h>
#include <st/graphics/camera.h>
#include <st/graphics/renderer2d.h>
#include <st/utils/print.h>
#include <st/utils/util.h>
#include <st/utils/vector.h>


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

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(StVertex2d), (void *)offsetof(StVertex2d, tex_coords));
    glEnableVertexAttribArray(2);

    // Texture index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
        sizeof(StVertex2d), (void *)offsetof(StVertex2d, tex_index));
    glEnableVertexAttribArray(3);

    const char *vertex_path = ST_ASSETS_PATH "/shaders/vertex_2d.glsl";
    const char *fragment_path = ST_ASSETS_PATH "/shaders/fragment_2d.glsl";
    renderer->gl.program = create_shader(vertex_path, fragment_path);
    assert(renderer->gl.program);

    st_debug("Shader created\n");
    st_debug("... id: %d\n", renderer->gl.program);
    st_debug("... vertex: '%s'\n", vertex_path);
    st_debug("... fragment: '%s'\n", fragment_path);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void gl_destroy(StRenderer2d *renderer)
{
    glDeleteBuffers(1, &renderer->gl.vbo);
    glDeleteVertexArrays(1, &renderer->gl.vao);
    glDeleteProgram(renderer->gl.program);

    vector_for(renderer->textures, StTexture *, texture)
        glDeleteTextures(1, &(*texture)->gl.id);
}

static void gl_begin(StRenderer2d *renderer)
{
    int width, height;
    st_window_get_size(&width, &height);
    glViewport(0, 0, width, height);

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

static void gl_flush(StRenderer2d *renderer)
{
    glBindVertexArray(renderer->gl.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gl.vbo);
    const int offset = renderer->vertex_pointer - renderer->vertex_buffer;
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(StVertex2d), renderer->vertex_buffer);

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
        assert(data);
    } else {
        data = texture->data.bytes;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width,
        texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (texture->from_file)
        stbi_image_free(data);
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

    st_debug("Vertex buffer created (capacity: %d vertices, %d bytes)\n",
        vector_capacity(renderer->vertex_buffer), 
        vector_capacity(renderer->vertex_buffer) * sizeof(StVertex2d));

    gl_init(renderer);

    // 1x1 white texture
    static StTexture white = {0};
    unsigned char bytes[] = {0xff, 0xff, 0xff, 0xff};
    st_texture_from_bytes(&white, bytes, 1, 1);
    st_renderer2d_add_texture(renderer, &white);

    st_debug("2D renderer created\n");
}

void st_renderer2d_destroy(StRenderer2d *renderer)
{
    if (!renderer)
        return;

    vector_free(renderer->vertex_buffer);

    gl_destroy(renderer);

    vector_for(renderer->textures, StTexture *, texture)
        st_texture_destroy(*texture);

    st_debug("2D renderer destroyed\n");
}

void st_renderer2d_add_texture(StRenderer2d *renderer, StTexture *texture)
{
    assert(renderer);
    assert(texture);
    assert(vector_length(renderer->textures) <= 4);

    vector_push(renderer->textures, texture);

    gl_init_texture(texture);

    st_debug("Texture added to 2D renderer\n");
    st_debug("... id: %d\n", texture->gl.id);
    st_debug("... size: %dx%d\n", texture->width, texture->height);
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

void st_renderer2d_draw_quad(StRenderer2d *renderer, vec3 position,
    vec3 rotation, vec3 scale, vec4 color)
{
    st_renderer2d_draw_textured_quad(renderer, position, rotation, scale, color,
        renderer->textures[0]);
}

static int index_from_id(StRenderer2d *renderer, GLuint id)
{
    for (size_t i = 0; i < vector_length(renderer->textures); i++) {
        if (renderer->textures[i]->gl.id == id)
            return (int)i;
    }
    return -1;
}

void st_renderer2d_draw_textured_quad(StRenderer2d *renderer, vec3 position,
    vec3 rotation, vec3 scale, vec4 color, StTexture *texture)
{
    const float index = (float)index_from_id(renderer, texture->gl.id);

    StVertex2d vertices[] = {
        {{ 5.0f,  5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {1.0f, 1.0f}, index}, // top right
        {{ 5.0f, -5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {1.0f, 0.0f}, index}, // bottom right
        {{-5.0f,  5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {0.0f, 1.0f}, index}, // top left

        {{ 5.0f, -5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {1.0f, 0.0f}, index}, // bottom right
        {{-5.0f, -5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}, index}, // bottom left
        {{-5.0f,  5.0f, 0.0f}, {color[0], color[1], color[2], color[3]}, {0.0f, 1.0f}, index}  // top left
    };

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, position);
    glm_rotate(model, glm_rad(rotation[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(rotation[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(model, scale);

    for (int i = 0; i < 6; i++) {
        glm_mat4_mulv3(model, vertices[i].position, 1.0f, vertices[i].position);
    }

    st_renderer2d_draw(renderer, vertices, 6);
}
