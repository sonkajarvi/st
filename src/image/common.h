#ifndef IMAGE_COMMON_H
#define IMAGE_COMMON_H

#include <stdio.h>
#include <stdint.h>

#include <st/utility/compiler.h>

#define CONTEXT_INIT(data, len) { data, len, 0 }

#define PNG_SIGNATURE 0x89504e470d0a1a0a

struct st_image;

struct context
{
    const uint8_t *data;
    const size_t length;
    size_t index;
};

uint8_t  read(struct context *ctx);
uint16_t read_16(struct context *ctx);
uint32_t read_32(struct context *ctx);
uint64_t read_64(struct context *ctx);

ST_NODISCARD uint8_t  peek(struct context *ctx);
ST_NODISCARD uint16_t peek_16(struct context *ctx);
ST_NODISCARD uint32_t peek_32(struct context *ctx);
ST_NODISCARD uint64_t peek_64(struct context *ctx);

int parse_png(struct st_image *img, struct context *ctx);

#endif // IMAGE_COMMON_H
