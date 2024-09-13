#ifndef ST_RENDERER_H
#define ST_RENDERER_H

#include <cglm/vec3.h>

#include <st/camera.h>
#include <st/model.h>

typedef struct StLight
{
    vec3 position;
    vec3 color;
} StLight;

void renderer_init(StCamera *camera, StLight *light);
void renderer_destroy(void);
void renderer_begin(void);
void renderer_end(void);
void renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count);

#endif // ST_RENDERER_H
