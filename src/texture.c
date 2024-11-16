#include <st/instance.h>
#include <st/graphics/texture.h>
#include <st/utility/assert.h>
#include <st/utility/print.h>

// todo: Return a status code
void st_texture_create(struct st_texture *tex, struct st_image *img)
{
    St *st;

    if (!tex || !img || !(st = st_instance()))
        return;

    call_impl(st, texture_create, tex, img);

    tex->width = img->width;
    tex->height = img->height;
    tex->index = -1;

    st_debug("Texture created (at %p) from %p\n", tex, img);
}

// todo: Return a status code
void st_texture_destroy(struct st_texture *tex)
{
    St *st;

    if (!tex || !(st = st_instance()))
        return;

    call_impl(st, texture_destroy, tex);

    st_debug("Texture destroyed (at %p)\n", tex);
}
