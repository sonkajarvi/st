#ifndef RENDERER_GL_H
#define RENDERER_GL_H

#include <st/common.h>

void impl_gl_renderer_init(StCamera *camera, StLight *light);
void impl_gl_renderer_destroy(void);
void impl_gl_renderer_begin(void);
void impl_gl_renderer_end(void);
void impl_gl_renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count);

#endif // RENDERER_GL_H
