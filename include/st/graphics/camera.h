#ifndef ST_CAMERA_H
#define ST_CAMERA_H

#include <cglm/vec3.h>
#include <cglm/mat4.h>

#include <st/util/assert.h>

// projection types
#define ST_CAMERA_PERSPECTIVE   0
#define ST_CAMERA_ORTHO         1

// clipping planes
#define ST_CAMERA_NEAR 0.1f
#define ST_CAMERA_FAR 100.0f

struct st_camera
{
    int type;
    float fov;
    vec2 viewport;

    vec3 position;
    vec3 rotation;

    mat4 view_mat;
    mat4 proj_mat;
};

void st_camera_recalculate_view(struct st_camera *camera, bool rotated);
void st_camera_recalculate_projection(struct st_camera *camera);

void st_camera_set_projection(struct st_camera *camera, int type);

void st_camera_set_position(struct st_camera *camera, float x, float y, float z);
void st_camera_add_position(struct st_camera *camera, float x, float y, float z);

void st_camera_set_rotation(struct st_camera *camera, float x, float y, float z);
void st_camera_add_rotation(struct st_camera *camera, float x, float y, float z);

void st_camera_set_fov(struct st_camera *camera, float fov);
void st_camera_add_fov(struct st_camera *camera, float fov);

static inline void st_camera_init(struct st_camera *const camera, int type)
{
    st_assert(camera);

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
