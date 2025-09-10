#version 430 core

out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D tex;

void main() {
    FragColor = vec4(texture(tex, tex_coords).rgb, 1.0);
}
