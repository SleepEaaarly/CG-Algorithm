#version 430 core

in vec3 pos;
out vec4 FragColor;

uniform int sh_degree;
uniform vec3 sh_lighting_coeffs[16];

const float a0 = 3.141593;
const float a1 = 2.094395;
const float a2 = 0.785398;
const float a3 = 0;

const float c10 = 0.488603; // 0.5 * std::sqrt(3) * inv_sqrt_pi
const float c20 = 1.092548; // 0.5 * std::sqrt(15) * inv_sqrt_pi
const float c21 = 0.315392; // 0.25 * std::sqrt(5) * inv_sqrt_pi
const float c22 = 0.546274; // 0.25 * std::sqrt(15) * inv_sqrt_pi
const float c30 = 0.373176;  // 0.25 * std::sqrt(7) * inv_sqrt_pi
const float c31 = 0.457046;  // 0.25 * std::sqrt(21.0/2.0) * inv_sqrt_pi
const float c32 = 1.445306;  // 0.25 * std::sqrt(105) * inv_sqrt_pi
const float c33 = 0.590044;  // 0.25 * std::sqrt(35.0/2.0) * inv_sqrt_pi
            
float getBasis00(float x, float y, float z) {return 0.282095;}
float getBasis10(float x, float y, float z) {return c10*z;}
float getBasis11(float x, float y, float z) {return c10*x;}
float getBasis1_1(float x, float y, float z) {return c10*y;}
float getBasis20(float x, float y, float z) {return c21*(2*z*z-x*x-y*y);}
float getBasis21(float x, float y, float z) {return c20*(z*x);}
float getBasis2_1(float x, float y, float z) {return c20*(y*z);}
float getBasis22(float x, float y, float z) {return c22*(x*x-y*y);}
float getBasis2_2(float x, float y, float z) {return c20*(x*y);}
float getBasis30(float x, float y, float z) {return c30 * z*(2*z*z-3*x*x-3*y*y);}
float getBasis31(float x, float y, float z) {return c31 * x*(5*z*z-1);}
float getBasis3_1(float x, float y, float z) {return c31 * y*(5*z*z-1);}
float getBasis32(float x, float y, float z) {return c32 * z*(x*x-y*y);}
float getBasis3_2(float x, float y, float z) {return 2 * c32 * x*y*z;}
float getBasis33(float x, float y, float z) {return c33 * x*(x*x-3*y*y);}
float getBasis3_3(float x, float y, float z) {return c33 * y*(3*x*x-y*y);}

vec3 calcSHLightingColor(vec3 dir) {
    vec3 color = vec3(0.0, 0.0, 0.0);
    float x = dir.x; float y = dir.y; float z = dir.z;
    if (sh_degree >= 0) {
        color = color + a0 * sh_lighting_coeffs[0] * getBasis00(x,y,z);
    }
    if (sh_degree >= 1) {
        color = color + a1 * sh_lighting_coeffs[1] * getBasis1_1(x,y,z);
        color = color + a1 * sh_lighting_coeffs[2] * getBasis10(x,y,z);
        color = color + a1 * sh_lighting_coeffs[3] * getBasis11(x,y,z);
    }
    if (sh_degree >= 2) {
        color = color + a2 * sh_lighting_coeffs[4] * getBasis2_2(x,y,z);
        color = color + a2 * sh_lighting_coeffs[5] * getBasis2_1(x,y,z);
        color = color + a2 * sh_lighting_coeffs[6] * getBasis20(x,y,z);
        color = color + a2 * sh_lighting_coeffs[7] * getBasis21(x,y,z);
        color = color + a2 * sh_lighting_coeffs[8] * getBasis22(x,y,z);
    }
    if (sh_degree == 3) {
        // a3 == 0
    }
    return color;
}

void main() {
    vec3 dir = normalize(pos);

    vec3 color = calcSHLightingColor(dir);

    // HDR tonemap and gamma correct
    color = color / (color + vec3(1.0));
    // color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}
