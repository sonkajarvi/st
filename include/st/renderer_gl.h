#ifndef RENDERER_GL_H
#define RENDERER_GL_H

#include <st/camera.h>
#include <st/renderer.h>

void impl_gl_renderer_init(StCamera *camera, StLight *light);
void impl_gl_renderer_destroy(void);
void impl_gl_renderer_begin(void);
void impl_gl_renderer_end(void);
void impl_gl_renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count);
void impl_gl_renderer_draw_model(StModel *model, mat4 *model_matrix, vec4 color, StCamera *camera, StLight *light);

#endif // RENDERER_GL_H
