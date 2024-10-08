#version 460 core

out vec4 o_color;

in vec4 v_color;
in vec2 v_tex_coords;
in float v_tex_index;

uniform sampler2D u_textures[4];

void main()
{
    vec4 color = v_color;

    switch (int(v_tex_index)) {
    case 0: color *= texture(u_textures[0], v_tex_coords); break;
    case 1: color *= texture(u_textures[1], v_tex_coords); break;
    case 2: color *= texture(u_textures[2], v_tex_coords); break;
    case 3: color *= texture(u_textures[3], v_tex_coords); break;
    }

    o_color = color;
}
