#include <stdio.h>

#include <glad/glad.h>
#include <cglm/vec2.h>
#include <cglm/vec4.h>

#include <common.h>
#include <glad_loader.h>
#include <vector.h>

int main(void)
{
    engine_init();
    window_create("rendertest", 1200, 800);

    glad_init();

    printf("GL version:   %s\n", glGetString(GL_VERSION));
    printf("GL renderer:  %s\n", glGetString(GL_RENDERER));
    printf("GL vendor:    %s\n", glGetString(GL_VENDOR));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    StCamera camera = {
        {0.0f, 0.0f, -5.0f}
    };

    StModel model = {0};
    model_from_obj(&model, ST_ASSETS_PATH "/models/suzanne.obj");

    renderer_init(&camera);

    window_show();
    while (!window_should_close()) {
        poll_events();

        renderer_begin();
        renderer_push_mesh(model.vertices, vector_length(model.vertices), NULL, 0);
        renderer_end();

        swap_buffers();
    }

    model_destroy(&model);
    renderer_destroy();
    window_destroy();
    engine_destroy();

    return 0;
}
