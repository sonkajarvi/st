#ifndef ST_MODEL_H
#define ST_MODEL_H

#include <cglm/vec3.h>
#include <cglm/vec4.h>

typedef struct StVertex
{
    vec3 position;
    vec4 color;
    vec3 normal;
} StVertex;

typedef struct StModel
{
    StVertex *vertices;
} StModel;

void model_from_obj(StModel *model, const char *path);
void model_destroy(StModel *model);

#endif // ST_MODEL_H
