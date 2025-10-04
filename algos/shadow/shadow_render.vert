#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_matrix;

out VS_OUT {
    vec3 world_pos;
    vec3 world_normal;
    vec2 tex_coords;
    vec4 light_space_pos;
} vs_out;

void main() {
    vec4 world_pos = model * vec4(pos, 1.0);
    vs_out.world_pos = vec3(world_pos);
    vs_out.world_normal = transpose(inverse(mat3(model))) * normal;
    vs_out.tex_coords = tex_coords;
    vs_out.light_space_pos = light_space_matrix * model * vec4(pos, 1.0);
    gl_Position = projection * view * world_pos;
}
