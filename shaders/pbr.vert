#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 tex_coords;
out vec3 world_pos;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;

void main() {
    tex_coords = aTexCoords;
    world_pos = vec3(model * vec4(aPos, 1.0));
    normal = normalize(normal_matrix * aNormal); 
    tangent =  normalize(mat3(model) * aTangent);
    bitangent = normalize(cross(normal, tangent));

    gl_Position =  projection * view * vec4(world_pos, 1.0);
}