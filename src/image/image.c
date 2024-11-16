#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <stb/stb_image.h>

#include <st/graphics/image.h>
#include <st/utility/print.h>

void st_image_from_file(struct st_image *img, const char *path)
{
    if (!img || !path || path[0] == '\0')
        return;

    stbi_set_flip_vertically_on_load(1);

    int channels;
    img->data = stbi_load(path, &img->width, &img->height, &channels, 4);

    st_debug("Image created from file (at %p)\n", img);
    st_debug("[data=%p, width=%d, height=%d]\n", img->data, img->width, img->height);
}

void st_image_free(struct st_image *img)
{
    if (!img)
        return;

    stbi_image_free(img->data);

    st_debug("Image freed (at %p)\n", img);
}
