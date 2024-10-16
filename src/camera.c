#include <assert.h>
#include <stdbool.h>

#include <cglm/vec3.h>
#include <cglm/cam.h>

#include <st/gfx/camera.h>
#include <st/utils/print.h>
#include <st/window.h>

static vec3 cam_front;
static vec3 cam_up = {0.0f, 1.0f, 0.0f};

void st_camera_recalculate_view(StCamera *camera, bool rotated)
{
    assert(camera);

    if (rotated) {
        cam_front[0] = cosf(glm_rad(camera->rotation[1])) * cosf(glm_rad(camera->rotation[0]));
        cam_front[1] = sinf(glm_rad(camera->rotation[0]));
        cam_front[2] = sinf(glm_rad(camera->rotation[1])) * cosf(glm_rad(camera->rotation[0]));
        glm_normalize(cam_front);
    }

    vec3 pos_and_front;
    glm_vec3_add(camera->position, cam_front, pos_and_front);

    glm_lookat(camera->position, pos_and_front, cam_up, camera->view_mat);
}

void st_camera_recalculate_projection(StCamera *camera)
{
    assert(camera);

    float ratio;
    int width, height;
    st_window_get_size(&width, &height);

    switch (camera->type) {
    case ST_CAMERA_PERSPECTIVE:
        ratio = (float)width / (float)height;
        glm_perspective(glm_rad(camera->fov), ratio, ST_CAMERA_NEAR, ST_CAMERA_FAR, camera->proj_mat);
        break;

    case ST_CAMERA_ORTHO:
        glm_ortho(0, width, 0, height, ST_CAMERA_NEAR, ST_CAMERA_FAR, camera->proj_mat);
        break;

    default:
        st_error("camera: unknown projection type: %d\n", camera->type);
        return;
    }
}

void st_camera_set_projection(StCamera *camera, int type)
{
    assert(camera);

    // todo: validate type
    camera->type = type;

    st_camera_recalculate_projection(camera);
}

void st_camera_set_position(StCamera *camera, float x, float y, float z)
{
    // todo: we could just return here instead
    assert(camera);

    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;

    st_camera_recalculate_view(camera, false);
}

void st_camera_add_position(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    vec3 tmp, cross;
    glm_cross(cam_front, cam_up, cross);
    glm_normalize(cross);

    glm_vec3_scale(cross, x, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    glm_vec3_scale(cam_up, y, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    glm_vec3_scale(cam_front, z, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);

    st_camera_recalculate_view(camera, false);
}

void st_camera_set_rotation(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    camera->rotation[0] = x;
    camera->rotation[1] = y;
    camera->rotation[2] = z;

    st_camera_recalculate_view(camera, true);
}

void st_camera_add_rotation(StCamera *camera, float x, float y, float z)
{
    assert(camera);

    camera->rotation[0] += x;
    camera->rotation[1] += y;
    camera->rotation[2] += z;

    st_camera_recalculate_view(camera, true);
}

void st_camera_set_fov(StCamera *camera, float fov)
{
    assert(camera);

    camera->fov = fov;

    st_camera_recalculate_projection(camera);
}

void st_camera_add_fov(StCamera *camera, float fov)
{
    assert(camera);

    camera->fov += fov;

    st_camera_recalculate_projection(camera);
}
