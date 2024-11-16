#ifndef IMPL_TEXTURE_GL_H
#define IMPL_TEXTURE_GL_H

#include <st/graphics/image.h>
#include <st/graphics/texture.h>

void impl_gl_texture_create(struct st_texture *tex, struct st_image *img);
void impl_gl_texture_destroy(struct st_texture *tex);

#endif // IMPL_TEXTURE_GL_H
