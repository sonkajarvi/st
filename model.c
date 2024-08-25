#include "common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

#define is_whitespace(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r')

// https://en.wikipedia.org/wiki/Wavefront_.obj_file
// https://cs418.cs.illinois.edu/website/text/obj.html

static void skip_until_newline(StParser *parser, FILE *fp)
{
    char c;
    while ((c = consume(parser, fp)) != EOF) {
        if (c == '\n')
            break;
    }
}

static void skip_whitespace(StParser *parser, FILE *fp)
{
    char c;
    while ((c = peek(fp)) != EOF) {
        if (!is_whitespace(c))
            break;

        consume(parser, fp);
    }
}

static void model_push_vertex(StModel *model, float x, float y, float z)
{
    assert(model);

    if (model->v_len == model->v_cap) {
        model->v_cap = model->v_cap ? model->v_cap * 2 : 8;
        model->vertices = realloc(model->vertices, sizeof(*model->vertices) * model->v_cap);
    }

    model->v_len++;
    StVertex *v = &model->vertices[model->v_len - 1];
    v->position[0] = x;
    v->position[1] = y;
    v->position[2] = z;
    v->color[0] = 1.0f;
    v->color[1] = 1.0f;
    v->color[2] = 1.0f;
    v->color[3] = 1.0f;
}

static void model_push_index(StModel *model, unsigned int index)
{
    assert(model);

    if (model->i_len == model->i_cap) {
        model->i_cap = model->i_cap ? model->i_cap * 2 : 8;
        model->indices = realloc(model->indices, sizeof(*model->indices) * model->i_cap);
    }

    model->i_len++;
    model->indices[model->i_len - 1] = index;    
}

static float parse_float(StParser *parser, FILE *fp)
{
    char buf[16];
    int i = 0;

    skip_whitespace(parser, fp);

    char c;
    while (i < 15 && (c = peek(fp), !is_whitespace(c))) {
        buf[i++] = c;
        consume(parser, fp);
    }
    buf[i] = '\0';

    float ret;
    sscanf(buf, "%f", &ret);
    return ret;
}

static unsigned int parse_int(StParser *parser, FILE *fp)
{
    char buf[16];
    int i = 0;

    skip_whitespace(parser, fp);

    char c;
    while (i < 15 && (c = peek(fp), !is_whitespace(c))) {
        buf[i++] = c;
        consume(parser, fp);
    }
    buf[i] = '\0';

    unsigned int ret;
    sscanf(buf, "%u", &ret);
    return ret;
}

static void parse_vertex(StModel *model, StParser *parser, FILE *fp)
{
    float x, y, z;
    x = parse_float(parser, fp);
    y = parse_float(parser, fp);
    z = parse_float(parser, fp);
    model_push_vertex(model, x, y, z);
    
    // ignore 4th value
    skip_until_newline(parser, fp);
}

static void parse_face(StModel *model, StParser *parser, FILE *fp)
{
    unsigned int i0, i1, i2;
    i0 = parse_int(parser, fp) - 1;
    i1 = parse_int(parser, fp) - 1;
    i2 = parse_int(parser, fp) - 1;
    model_push_index(model, i0);
    model_push_index(model, i1);
    model_push_index(model, i2);

    while (peek(fp) != '\n') {
        i1 = i2;
        i2 = parse_int(parser, fp) - 1;
        model_push_index(model, i0);
        model_push_index(model, i1);
        model_push_index(model, i2);
    }

    skip_until_newline(parser, fp);
}

void model_from_obj(StModel *model, const char *path)
{
    assert(model);
    assert(path);

    FILE *fp = fopen(path, "r");
    assert(fp);

    StParser parser = {0};

    char c;
    while((c = consume(&parser, fp)) != EOF)
    {
        switch (c) {
        case '\n':
            printf("(%lld:%lld) empty line\n", parser.ln, parser.col);
            break;

        case '#':
            printf("(%lld:%lld) comment\n", parser.ln, parser.col);
            skip_until_newline(&parser, fp);
            break;

        case 'v':
            parse_vertex(model, &parser, fp);
            break;
        
        case 'f':
            parse_face(model, &parser, fp);
            break;
        
        default:
            printf("(%lld:%lld) unknown token: '%c' (0x%x)\n", parser.ln, parser.col, c, c);
            goto end;
        }
    }

    printf("finished parsing '%s'\n", path);

end:
    fclose(fp);
}

void model_destroy(StModel *model)
{
    assert(model);

    free(model->vertices);
    free(model->indices);
}
