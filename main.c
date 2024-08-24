#include <stdio.h>

#include <glad/glad.h>
#include <cglm/vec2.h>
#include <cglm/vec4.h>

#include "common.h"
#include "glad_loader.h"

int main(void)
{
    engine_init();
    window_create("rendertest", 1200, 800);

    glad_init();

    printf("GL version:   %s\n", glGetString(GL_VERSION));
    printf("GL renderer:  %s\n", glGetString(GL_RENDERER));
    printf("GL vendor:    %s\n", glGetString(GL_VENDOR));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    const float n = 0.25f;
    const struct st_vertex vertices[] = {
        { { n,  n, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
        { { n, -n, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
        { {-n, -n, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        { {-n,  n, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    };

    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    struct st_camera camera = {
        {0.0f, 0.0f, -3.0f}
    };
    
    renderer_init(&camera);

    window_show();
    while (!window_should_close()) {
        poll_events();

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
