// #include <stdbool.h>
// #include <stdint.h>
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <st/graphics/image.h>
#include <st/utility/print.h>
#include <st/utility/list.h>

#include "common.h"

#define IHDR 0x49484452
#define IDAT 0x49444154
#define IEND 0x49454e44

static int ihdr_validate_color(uint8_t type, uint8_t depth)
{
    switch (type) {
    case 0: 
        return depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16;
    
    case 3:
        return depth == 1 || depth == 2 || depth == 4 || depth == 8;
    
    case 2:
    case 4:
    case 6:
        return depth == 8 || depth == 16;
    
    default:
        return 0;
    }
}

struct png
{
    struct {
        uint32_t width;
        uint32_t height;
        uint8_t  depth;
        uint8_t  type;
        uint8_t  compression;
        uint8_t  filter;
        uint8_t  interlace;
    } ihdr;

    // Final PNG datastream
    uint8_t *data;
    size_t length;
};

static char *__type_as_str(const uint32_t type)
{
    static char s[5] = { 0 };
    s[0] = (type >> 24) & 0xff;
    s[1] = (type >> 16) & 0xff;
    s[2] = (type >> 8) & 0xff;
    s[3] = type & 0xff;
    return s;
}

static int parse_ihdr(struct context *ctx, struct png *png, const uint32_t length)
{
    if (length != 13)
        return 1;

    if ((png->ihdr.width = read_32(ctx)) == 0 ||
        (png->ihdr.height = read_32(ctx)) == 0)
        return 1;

    if (!ihdr_validate_color((png->ihdr.depth = read(ctx)), (png->ihdr.type = read(ctx))))
        return 1;

    png->ihdr.compression = read(ctx);
    png->ihdr.filter = read(ctx);
    png->ihdr.interlace = read(ctx);

    st_log("PNG: width......: %d\n", png->ihdr.width);
    st_log("PNG: height.....: %d\n", png->ihdr.height);
    st_log("PNG: depth......: %d\n", png->ihdr.depth);
    st_log("PNG: type.......: %d\n", png->ihdr.type);
    st_log("PNG: compression: %d\n", png->ihdr.compression);
    st_log("PNG: filter.....: %d\n", png->ihdr.filter);
    st_log("PNG: interlace..: %d\n", png->ihdr.interlace);

    return 0;
}

static int parse_idat(struct context *ctx, struct png *png, const uint32_t length)
{
    if ((png->data = realloc(png->data, png->length + length)) == NULL)
        return 1;
    
    memcpy(png->data + png->length, ctx->data + ctx->index, length);
    ctx->index += length;

    return 0;
}

static int parse_iend(const uint32_t length)
{
    if (length != 0)
        return 1;
    
    return -1;
}

static int parse_chunk(struct context *ctx, struct png *png)
{
    int ret;

    uint32_t length = read_32(ctx);
    uint32_t type = read_32(ctx);
    st_debug("PNG: Found chunk '%s'\n", __type_as_str(type));

    // IHDR must be the first chunk
    if (png->ihdr.width == 0 && type != IHDR)
        return 1;

    switch (type) {
    case IHDR:
        ret = parse_ihdr(ctx, png, length);
        break;
    
    case IDAT:
        ret = parse_idat(ctx, png, length);
        break;
    
    case IEND:
        ret = parse_iend(length);
        break;
    }

    // todo: Verify CRC
    read_32(ctx);

    return ret;
}

// Note: @img and @ctx are guaranteed to be non-null by the caller
int parse_png(struct st_image *img, struct context *ctx)
{
    int ret;
    struct png png = {0};

    // Signature was checked by the caller, so we can skip it
    read_64(ctx);

    while (1) {
        if ((ret = parse_chunk(ctx, &png)) != 0)
            break;
    }

    // -1 is used for IEND, but isn't an error
    if (ret == -1)
        ret = 0;

    img->width = png.ihdr.width;
    img->height = png.ihdr.height;

    st_log("PNG: data: %p\n", png.data);
    free(png.data);

    return ret;
}
