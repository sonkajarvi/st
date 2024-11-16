#ifndef ST_GRAPHICS_IMAGE_H
#define ST_GRAPHICS_IMAGE_H

// Note: Uses RGBA format
struct st_image
{
    unsigned char *data;
    int width;
    int height;
};

void st_image_from_file(struct st_image *img, const char *path);
void st_image_free(struct st_image *img);

#endif // ST_GRAPHICS_IMAGE_H
