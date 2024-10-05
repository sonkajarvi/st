#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 u_projection_mat;
uniform mat4 u_view_mat;

void main()
{
    v_color = a_color;
    gl_Position = u_projection_mat * u_view_mat * vec4(a_position, 1.0);
}
