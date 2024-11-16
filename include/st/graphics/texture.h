#ifndef ST_GRAPHICS_TEXTURE_H
#define ST_GRAPHICS_TEXTURE_H

#include <glad/gl.h>

#include <st/graphics/image.h>

struct st_texture
{
    int width;
    int height;
    int index;

    struct {
        GLuint id;
    } gl;
};

void st_texture_create(struct st_texture *tex, struct st_image *img);
void st_texture_destroy(struct st_texture *tex);

#endif // ST_GRAPHICS_TEXTURE_H
