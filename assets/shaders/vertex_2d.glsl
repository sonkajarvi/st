#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in float a_tex_index;

out vec4 v_color;
out vec2 v_tex_coords;
out float v_tex_index;

uniform mat4 u_projection_mat;
uniform mat4 u_view_mat;

void main()
{
    v_color = a_color;
    v_tex_coords = a_tex_coords;
    v_tex_index = a_tex_index;

    gl_Position = u_projection_mat * u_view_mat * vec4(a_position, 1.0);
}
