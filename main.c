#include <stdio.h>

#include <GL/gl.h>
#include <GL/glext.h>

#include "common.h"

int main(void)
{
    engine_init();
    window_create("rendertest", 150, 150);

    printf("GL version:   %s\n", glGetString(GL_VERSION));
    printf("GL renderer:  %s\n", glGetString(GL_RENDERER));
    printf("GL vendor:    %s\n", glGetString(GL_VENDOR));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    int w, h, x, y;
    
    window_show();
    while (!window_should_close()) {
        poll_events();

        window_get_size(&w, &h);
        printf("window size: %d %d, ", w, h);
        
        window_get_pos(&x, &y);
        printf("window pos: %d %d\n", x, y);

        glClear(GL_COLOR_BUFFER_BIT);
        swap_buffers();
    }

    window_destroy();
    engine_destroy();

    return 0;
}
