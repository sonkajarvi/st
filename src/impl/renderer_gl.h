#ifndef IMPL_RENDERER_GL_H
#define IMPL_RENDERER_GL_H

#include <stddef.h>

#include <st/window.h>
#include <st/graphics/renderer.h>

void impl_gl_renderer_init(struct st_window *window, StRenderer *renderer, StCamera *camera);
void impl_gl_renderer_destroy(struct st_window *window, StRenderer *renderer);
void impl_gl_renderer_add_texture(
    struct st_window *window, StRenderer *renderer, StTexture *texture);

void impl_gl_draw_begin(struct st_window *window, StRenderer *renderer);
void impl_gl_draw_end(struct st_window *window, StRenderer *renderer);

void impl_gl_renderer_push(struct st_window * window,
    StRenderer * renderer, StVertex *vertices, size_t count);

#endif // IMPL_RENDERER_GL_H
