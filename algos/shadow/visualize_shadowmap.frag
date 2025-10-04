#version 430 core

in vec2 a_tex_coords;

uniform sampler2D shadow_map;

void main() {
    float depth = texture(shadow_map, a_tex_coords).r;
    gl_FragColor = vec4(vec3(depth), 1.0);
    // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
