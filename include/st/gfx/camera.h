#ifndef ST_CAMERA_H
#define ST_CAMERA_H

#include <cglm/vec3.h>
#include <cglm/mat4.h>

// projection types
#define ST_CAMERA_PERSPECTIVE   0
#define ST_CAMERA_ORTHO         1

// clipping planes
#define ST_CAMERA_NEAR 0.1f
#define ST_CAMERA_FAR 100.0f

typedef struct StCamera
{
    int type;
    float fov;

    vec3 position;
    vec3 rotation;

    mat4 view_mat;
    mat4 proj_mat;
} StCamera;

void st_camera_recalculate_view(StCamera *camera, bool rotated);
void st_camera_recalculate_projection(StCamera *camera);

void st_camera_set_projection(StCamera *camera, int type);

void st_camera_set_position(StCamera *camera, float x, float y, float z);
void st_camera_add_position(StCamera *camera, float x, float y, float z);

void st_camera_set_rotation(StCamera *camera, float x, float y, float z);
void st_camera_add_rotation(StCamera *camera, float x, float y, float z);

void st_camera_set_fov(StCamera *camera, float fov);
void st_camera_add_fov(StCamera *camera, float fov);

static inline void st_camera_init(StCamera *const camera, int type)
{
    assert(camera);

    switch (type) {
    case ST_CAMERA_ORTHO:
        camera->type = ST_CAMERA_ORTHO;
        glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, camera->position);
        glm_vec3_copy((vec3){0.0f, -90.0f, 0.0f}, camera->rotation);
        break;
    }

    st_camera_recalculate_view(camera, true);
    st_camera_recalculate_projection(camera);
}

#endif // ST_CAMERA_H
