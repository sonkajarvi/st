#include "common.h"

uint8_t read(struct context *ctx)
{
    uint8_t ret = 0;
    if (ctx->index < ctx->length)
        ret = ctx->data[ctx->index++];
    return ret;
}

uint16_t read_16(struct context *ctx)
{
    uint16_t ret = 0;
    if (ctx->index + 1 < ctx->length) {
        ret |= (uint16_t)ctx->data[ctx->index++] << 8;
        ret |= ctx->data[ctx->index++];
    }
    return ret;
}

uint32_t read_32(struct context *ctx)
{
    uint32_t ret = 0;
    if (ctx->index + 3 < ctx->length) {
        for (int i = 24; i >= 0; i -= 8)
            ret |= (uint32_t)ctx->data[ctx->index++] << i;
    }
    return ret;
}

uint64_t read_64(struct context *ctx)
{
    uint64_t ret = 0;
    if (ctx->index + 7 < ctx->length) {
        for (int i = 56; i >= 0; i -= 8)
            ret |= (uint64_t)ctx->data[ctx->index++] << i;
    }
    return ret;
}

uint8_t peek(struct context *ctx)
{
    uint8_t ret = 0;
    if (ctx->index < ctx->length)
        ret = ctx->data[ctx->index];
    return ret;
}

uint16_t peek_16(struct context *ctx)
{
    uint16_t ret = 0;
    if (ctx->index + 1 < ctx->length) {
        ret |= (uint16_t)ctx->data[ctx->index] << 8;
        ret |= ctx->data[ctx->index + 1];
    }
    return ret;
}

uint32_t peek_32(struct context *ctx)
{
    uint32_t ret = 0;
    if (ctx->index + 3 < ctx->length) {
        for (int i = 24, j = 0; i >= 0; i -= 8, j++)
            ret |= (uint32_t)ctx->data[ctx->index + j] << i;
    }
    return ret;
}

uint64_t peek_64(struct context *ctx)
{
    uint64_t ret = 0;
    if (ctx->index + 7 < ctx->length) {
        for (int i = 56, j = 0; i >= 0; i -= 8, j++)
            ret |= (uint64_t)ctx->data[ctx->index + j] << i;
    }
    return ret;
}
