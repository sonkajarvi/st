#ifndef ST_GRAPHICS_VERTEX_H
#define ST_GRAPHICS_VERTEX_H

#include <cglm/vec3.h>
#include <cglm/vec4.h>

typedef struct StVertex
{
    vec3 position;
    vec4 color;
} StVertex;

#endif // ST_GRAPHICS_VERTEX_H
