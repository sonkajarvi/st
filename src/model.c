#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include <st/model.h>
#include <st/parse.h>
#include <st/vector.h>

#define is_whitespace(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r')

// https://en.wikipedia.org/wiki/Wavefront_.obj_file
// https://cs418.cs.illinois.edu/website/text/obj.html

typedef struct StModel_OBJBuffer
{
    vec3 *vertices;
    vec3 *normals;
} StModel_OBJBuffer;

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
    while (i < 15 && (c = peek(fp), c >= '0' && c <= '9')) {
        buf[i++] = c;
        consume(parser, fp);
    }
    buf[i] = '\0';

    unsigned int ret;
    sscanf(buf, "%u", &ret);
    return ret;
}

static void parse_vertex(StModel *model, StParser *parser, FILE *fp, StModel_OBJBuffer *buffer)
{
    (void)model;
    (void)buffer;

    vec3 v;
    v[0] = parse_float(parser, fp);
    v[1] = parse_float(parser, fp);
    v[2] = parse_float(parser, fp);
    vector_push_copy(buffer->vertices, v);
    
    // ignore 4th value
    skip_until_newline(parser, fp);
}

static void parse_normal(StModel *model, StParser *parser, FILE *fp, StModel_OBJBuffer *buffer)
{
    (void)model;
    (void)buffer;

    vec3 v;
    v[0] = parse_float(parser, fp);
    v[1] = parse_float(parser, fp);
    v[2] = parse_float(parser, fp);
    vector_push_copy(buffer->normals, v);
    
    // ignore 4th value
    skip_until_newline(parser, fp);
}

static void model_push_vertex(StModel *model, StModel_OBJBuffer *buffer, unsigned int vertex, unsigned int normal)
{
    (void)model;
    (void)buffer;
    (void)vertex;
    (void)normal;

    assert(vertex < vector_length(buffer->vertices));
    assert(normal < vector_length(buffer->normals));

    StVertex v;
    v.position[0] = buffer->vertices[vertex][0];
    v.position[1] = buffer->vertices[vertex][1];
    v.position[2] = buffer->vertices[vertex][2];
    
    v.color[0] = 1.0f;
    v.color[1] = 1.0f;
    v.color[2] = 1.0f;
    v.color[3] = 1.0f;
    
    v.normal[0] = buffer->normals[normal][0];
    v.normal[1] = buffer->normals[normal][1];
    v.normal[2] = buffer->normals[normal][2];

    vector_push_copy(model->vertices, v);
}

static void parse_face(StModel *model, StParser *parser, FILE *fp, StModel_OBJBuffer *buffer)
{
    (void)model;
    (void)buffer;

    unsigned int v0, v1, v2;
    unsigned int n0, n1, n2;

    v0 = parse_int(parser, fp) - 1;
    consume(parser, fp);
    consume(parser, fp);
    n0 = parse_int(parser, fp) - 1;
    
    v1 = parse_int(parser, fp) - 1;
    consume(parser, fp);
    consume(parser, fp);
    n1 = parse_int(parser, fp) - 1;

    v2 = parse_int(parser, fp) - 1;
    consume(parser, fp);
    consume(parser, fp);
    n2 = parse_int(parser, fp) - 1;

    model_push_vertex(model, buffer, v0, n0);
    model_push_vertex(model, buffer, v1, n1);
    model_push_vertex(model, buffer, v2, n2);

    while (peek(fp) != '\n') {
        v1 = v2;
        n1 = n2;

        v2 = parse_int(parser, fp) - 1;
        consume(parser, fp);
        consume(parser, fp);
        n2 = parse_int(parser, fp) - 1;

        model_push_vertex(model, buffer, v0, n0);
        model_push_vertex(model, buffer, v1, n1);
        model_push_vertex(model, buffer, v2, n2);
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
    StModel_OBJBuffer buffer = {0};

    char c;
    while((c = consume(&parser, fp)) != EOF)
    {
        switch (c) {
        case '\n':
            parser_printf(parser, "empty line\n");
            break;

        case '#':
            parser_printf(parser, "comment\n");
            skip_until_newline(&parser, fp);
            break;

        case 'v':
            c = consume(&parser, fp);
            switch (c) {
            case ' ':
                // parser_printf(parser, "vertex\n");
                parse_vertex(model, &parser, fp, &buffer);
                break;
            
            case 'n':
                // parser_printf(parser, "normal\n");
                parse_normal(model, &parser, fp, &buffer);
                break;
            
            default:
                parser_printf(parser, "unknown token: '%c' (0x%x)\n", c, c);
                goto end;
            }
            break;
        
        case 'f':
            // parser_printf(parser, "face\n");
            parse_face(model, &parser, fp, &buffer);
            break;
        
        default:
            parser_printf(parser, "unknown token: '%c' (0x%x)\n", c, c);
            goto end;
        }
    }

    printf("finished parsing '%s'\n", path);
    printf("vertices: %zu\n", vector_length(buffer.vertices));
    printf("normals: %zu\n", vector_length(buffer.normals));

end:
    fclose(fp);

    vector_free(buffer.vertices);
    vector_free(buffer.normals);
}

void model_destroy(StModel *model)
{
    assert(model);

    free(model->vertices);
}
