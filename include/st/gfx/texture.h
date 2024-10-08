#ifndef ST_GFX_TEXTURE_H
#define ST_GFX_TEXTURE_H

#include <stdbool.h>

#include <glad/gl.h>

typedef struct StTexture
{
    int width;
    int height;
    bool from_file;
    union {
        char *path;
        struct  {
            unsigned char *bytes;
        };
    } data;
    struct {
        GLuint id;
    } gl;
} StTexture;

void st_texture_from_file(StTexture *texture, const char *path);
void st_texture_from_bytes(StTexture *texture, unsigned char *bytes, int width, int height);

void st_texture_destroy(StTexture *texture);

#endif // ST_GFX_TEXTURE_H
