#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <st/gfx/texture.h>
#include <st/utils/print.h>

void st_texture_from_file(StTexture *texture, const char *path)
{
    assert(texture);

    texture->data.path = strdup(path);
    assert(texture->data.path);
    texture->width = -1;
    texture->height = -1;
    texture->from_file = true;

    texture->gl.id = 0;

    st_debug("Texture initialized from file (path='%s')\n", texture->data.path);
}

void st_texture_from_bytes(StTexture *texture, unsigned char *bytes, int width, int height)
{
    assert(texture);
    assert(bytes);

    texture->data.bytes = bytes;
    texture->width = width;
    texture->height = height;
    texture->from_file = false;

    texture->gl.id = 0;

    st_debug("Texture initialized from bytes (bytes=%p, width=%d, height=%d)\n", bytes, width, height);
}

void st_texture_destroy(StTexture *texture)
{
    assert(texture);

    if (texture->from_file)
        free(texture->data.path);

    st_debug("Texture destroyed (texture=%p)\n", texture);
}
