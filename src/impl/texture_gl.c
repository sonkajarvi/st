#include <glad/gl.h>

#include "texture_gl.h"

void impl_gl_texture_create(struct st_texture *tex, struct st_image *img)
{
    if (!tex || !img)
        return;

    glGenTextures(1, &tex->gl.id);
    glBindTexture(GL_TEXTURE_2D, tex->gl.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void impl_gl_texture_destroy(struct st_texture *tex)
{
    if (!tex)
        return;

    glDeleteTextures(1, &tex->gl.id);
}
