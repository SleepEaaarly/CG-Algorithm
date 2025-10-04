#version 430 core

in VS_OUT {
    vec3 world_pos;
    vec3 world_normal;
    vec2 tex_coords;
    vec4 light_space_pos;
} fs_in;

uniform sampler2D shadow_map;
uniform sampler2D wood_texture;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 eye_pos;

float getShadow(vec4 clip_pos) {
    vec3 light_space_pos = clip_pos.xyz / clip_pos.w;
    light_space_pos = light_space_pos * 0.5 + 0.5;
    float shadow_depth = texture(shadow_map, light_space_pos.xy).r;

    float shading_depth = light_space_pos.z;

    // if shading point deeper than far plane of light space, return no shadow 
    if (shading_depth > 1.0)
        return 0.0;

    if (shadow_depth < shading_depth)
        return 1.0;
    return 0.0;

}

void main() {
    vec3 albedo = texture(wood_texture, fs_in.tex_coords).rgb;
    vec3 amb = vec3(0.3) * light_color;

    vec3 normal = normalize(fs_in.world_normal);
    vec3 l = normalize(light_pos - fs_in.world_pos);
    vec3 v = normalize(eye_pos - fs_in.world_pos);
    vec3 h = normalize(v+l);

    float diff = max(0.0, dot(normal, l));
    float spec = pow(max(dot(normal, h), 0.0), 64.0);

    float shadow = getShadow(fs_in.light_space_pos);
    vec3 lighting = (amb + (1 - shadow) * (diff + spec)) * albedo;

    gl_FragColor = vec4(lighting, 1.0);
}
