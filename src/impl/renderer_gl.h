#ifndef IMPL_RENDERER_GL_H
#define IMPL_RENDERER_GL_H

#include <stddef.h>

#include <st/window.h>
#include <st/graphics/renderer.h>

void impl_gl_renderer_init(StWindow *window, StRenderer *renderer, StCamera *camera);
void impl_gl_renderer_destroy(StWindow *window, StRenderer *renderer);
void impl_gl_renderer_add_texture(
    StWindow *window, StRenderer *renderer, StTexture *texture);

void impl_gl_draw_begin(StWindow *window, StRenderer *renderer);
void impl_gl_draw_end(StWindow *window, StRenderer *renderer);

void impl_gl_renderer_push(StWindow * window,
    StRenderer * renderer, StVertex *vertices, size_t count);

#endif // IMPL_RENDERER_GL_H
