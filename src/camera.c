#include <assert.h>
#include <stdbool.h>

#include <cglm/vec3.h>
#include <cglm/cam.h>

#include <st/camera.h>
#include <st/window.h>

static vec3 cam_front;
static vec3 cam_up = {0.0f, 1.0f, 0.0f};

static void recalculate_view(StCamera *camera)
{
    cam_front[0] = cosf(glm_rad(camera->rotation[1])) * cosf(glm_rad(camera->rotation[0]));
    cam_front[1] = sinf(glm_rad(camera->rotation[0]));
    cam_front[2] = sinf(glm_rad(camera->rotation[1])) * cosf(glm_rad(camera->rotation[0]));
    glm_normalize(cam_front);

    vec3 pos_and_front;
    glm_vec3_add(camera->position, cam_front, pos_and_front);

    glm_lookat(camera->position, pos_and_front, cam_up, camera->view_mat);
}

static void recalculate_projection(StCamera *camera)
{
    int width, height;
    window_get_size(&width, &height);
    const float ratio = (float)width / (float)height;
    glm_perspective(glm_rad(camera->fov), ratio, 0.1f, 100.0f, camera->proj_mat);
}

void camera_set_position(StCamera *camera, float x, float y, float z)
{
    // todo: we could just return here instead
    assert(camera);

    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;

    recalculate_view(camera);
}

void camera_add_position(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    vec3 tmp, cross;
    glm_cross(cam_front, cam_up, cross);
    glm_normalize(cross);

    glm_vec3_scale(cross, x, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    glm_vec3_scale(cam_front, y, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    glm_vec3_scale(cam_up, z, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    recalculate_view(camera);
}

void camera_set_rotation(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    camera->rotation[0] = x;
    camera->rotation[1] = y;
    camera->rotation[2] = z;

    recalculate_view(camera);
}

void camera_add_rotation(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    camera->rotation[0] += x;
    camera->rotation[1] += y;
    camera->rotation[2] += z;

    recalculate_view(camera);
}

void camera_set_fov(StCamera *camera, float fov)
{
    assert(camera);

    camera->fov = fov;

    recalculate_projection(camera);
}

void camera_add_fov(StCamera *camera, float fov)
{
    assert(camera);

    camera->fov += fov;

    recalculate_projection(camera);
}
