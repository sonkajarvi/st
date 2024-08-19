#include <stdio.h>

#include <glad/glad.h>
#include <cglm/struct/vec2.h>
#include <cglm/struct/vec4.h>

#include "common.h"
#include "glad_loader.h"

int main(void)
{
    engine_init();
    window_create("rendertest", 150, 150);

    glad_init();

    printf("GL version:   %s\n", glGetString(GL_VERSION));
    printf("GL renderer:  %s\n", glGetString(GL_RENDERER));
    printf("GL vendor:    %s\n", glGetString(GL_VENDOR));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    renderer_init();

    const struct st_vertex vertices[] = {
        { (vec2s){{ 0.5f,  0.5f}}, (vec4s){{1.0f, 1.0f, 1.0f, 1.0f}} },
        { (vec2s){{ 0.5f, -0.5f}}, (vec4s){{1.0f, 1.0f, 1.0f, 1.0f}} },
        { (vec2s){{-0.5f, -0.5f}}, (vec4s){{1.0f, 1.0f, 1.0f, 1.0f}} },
        { (vec2s){{-0.5f,  0.5f}}, (vec4s){{1.0f, 1.0f, 1.0f, 1.0f}} },
    };

    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    int width, height;

    window_show();
    while (!window_should_close()) {
        poll_events();
    
        window_get_size(&width, &height);
        glViewport(0, 0, width, height);

        renderer_begin();
        renderer_push_mesh(vertices, 4, indices, 6);
        renderer_end();

        swap_buffers();
    }

    renderer_destroy();
    window_destroy();
    engine_destroy();

    return 0;
}
