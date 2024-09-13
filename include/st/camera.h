#ifndef ST_CAMERA_H
#define ST_CAMERA_H

#include <cglm/vec3.h>
#include <cglm/mat4.h>

typedef struct StCamera
{
    vec3 position;
    vec3 rotation;
    float fov;
    mat4 view_mat;
    mat4 proj_mat;
} StCamera;

void camera_set_position(StCamera *camera, float x, float y, float z);
void camera_add_position(StCamera *camera, float x, float y, float z);

void camera_set_rotation(StCamera *camera, float x, float y, float z);
void camera_add_rotation(StCamera *camera, float x, float y, float z);

void camera_set_fov(StCamera *camera, float fov);
void camera_add_fov(StCamera *camera, float fov);

#endif // ST_CAMERA_H
