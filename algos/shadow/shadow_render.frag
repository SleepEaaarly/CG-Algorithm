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

float getShadow(vec4 clip_pos, vec3 n, vec3 l) {
    vec3 light_space_pos = clip_pos.xyz / clip_pos.w;
    light_space_pos = light_space_pos * 0.5 + 0.5;

    float shading_depth = light_space_pos.z;

    // if shading point deeper than far plane of light space, return no shadow 
    if (shading_depth > 1.0)
        return 0.0;

    float bias = max(0.05 * (1.0 - dot(n, l)), 0.005);
    float shadow = 0.0;

    vec2 texel_unit = 1.0 / textureSize(shadow_map, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float shadow_depth = texture(shadow_map, light_space_pos.xy + vec2(x, y) * texel_unit).r;
            if (shadow_depth + bias < shading_depth)
                shadow += 1.0;
        }
    }
    return shadow / 9.0;

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

    float shadow = getShadow(fs_in.light_space_pos, normal, l);
    vec3 lighting = (amb + (1 - shadow) * (diff + spec)) * albedo;

    gl_FragColor = vec4(lighting, 1.0);
}
