#version 460 core

out vec4 o_Color;

in vec4 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_LightColor;

    vec3 norm = normalize(v_Normal);
    vec3 light_dir = normalize(u_LightPos - v_FragPos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    vec3 result = (ambient + diffuse) * v_Color.xyz;
    o_Color = vec4(result, 1.0);
    // o_Color = vec4(v_Normal * 0.5 + 0.5, 1.0);
}
