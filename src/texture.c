#include <stdlib.h>
#include <string.h>

#include <st/graphics/texture.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>

void st_texture_from_file(StTexture *texture, const char *path)
{
    st_assert(texture);

    texture->data.path = strdup(path);
    st_assert(texture->data.path);
    texture->width = -1;
    texture->height = -1;
    texture->from_file = true;

    texture->gl.id = 0;

    st_debug("Texture initialized from file\n");
    st_debug("... path: '%s'\n", texture->data.path);
}

void st_texture_from_bytes(StTexture *texture, unsigned char *bytes, int width, int height)
{
    st_assert(texture);
    st_assert(bytes);

    texture->data.bytes = bytes;
    texture->width = width;
    texture->height = height;
    texture->from_file = false;

    texture->gl.id = 0;

    st_debug("Texture initialized from bytes\n");
    st_debug("... bytes: %p\n", bytes);
    st_debug("... size: %dx%d\n", width, height);
}

void st_texture_destroy(StTexture *texture)
{
    st_assert(texture);

    bool did_free = false;

    if (texture->from_file) {
        free(texture->data.path);
        did_free = true;
    }

    st_debug("Texture destroyed\n");
    st_debug("... texture: %p\n", texture);
    st_debug("... freed: %d\n", did_free);
}
