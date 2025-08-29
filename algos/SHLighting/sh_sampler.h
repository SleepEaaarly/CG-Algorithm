#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_image_write.h>
#include <string>
#include <vector>

class SHSampler {
  private:
    size_t degree;
    std::vector<glm::vec3> sh_coeffs;

    std::string path;
    bool has_data;

    bool read_n_floats() {
        std::ifstream fin(path);
        if (!fin) {
            return false;
        }
        auto n = (degree+1) * (degree+1);
        float x, y, z;
        while (fin >> x >> y >> z) {
            sh_coeffs.push_back(glm::vec3(x, y, z));
        }

        return sh_coeffs.size() == n;
    }

  public:
    SHSampler(size_t deg, const char *path) : degree(deg), path(path) {
        has_data = read_n_floats();
    }

    bool hasData() { return has_data; }

    void calcSHCoeffs(std::vector<float> &image_data, unsigned int width,
                      unsigned int height, int com = 3) {
        // com = 3 : rgb
        int n = (degree+1)*(degree+1);
        sh_coeffs.resize(n);
        std::vector<float> sh_basis(n);
        for (int face = 0; face < 6; ++face) {
            for (auto row = 0; row < height; ++row) {
                for (auto col = 0; col < width; ++col) {
                    auto ind =
                        (col + width * row + width * height * face) * com;
                    glm::vec3 color(image_data[ind], image_data[ind + 1],
                                    image_data[ind + 2]);
                    auto [world_vec, half_texel_vec] = getTexelWorldVector(face, row, col, height, width);
                    for (int l = 0; l <= degree; ++l) {
                        for (int m = -l; m <= l; ++m) {
                            int ind = l*(l+1)+m;
                            sh_basis[ind] = getBasis(world_vec, l, m);
                        }
                    }
                }
            }
        }
    }

    std::tuple<glm::vec3, glm::vec3> getTexelWorldVector(int face, unsigned int row, unsigned int col, unsigned int height, unsigned int width) {
        glm::vec3 dir;
        glm::vec3 unit;
        float tex_height_unit = 1.f / height;
        float tex_width_unit = 1.f / width;
        float u = (static_cast<float>(col) + 0.5f) * tex_width_unit;
        float v = (static_cast<float>(row) + 0.5f) * tex_height_unit;
        switch (face) {
        case 0:
            dir = glm::vec3(1.f, 1.f-2*u, 1.f-2*v);
            unit = glm::vec3(0.f, -tex_width_unit, -tex_height_unit);
            break;
        case 1:
            dir = glm::vec3(-1.f, 2*u-1.f, 1.f-2*v);
            unit = glm::vec3(0.f, tex_width_unit, -tex_height_unit);
            break;
        case 2:
            dir = glm::vec3(2*u-1.f, 1.f, 2*v-1.f);
            unit = glm::vec3(tex_width_unit, 0.f, tex_height_unit);
            break;
        case 3:
            dir = glm::vec3(2*u-1.f, -1.f, 1.f-2*v);
            unit = glm::vec3(tex_width_unit, 0.f, -tex_height_unit);
            break;
        case 4:
            dir = glm::vec3(2*u-1.f, 1.f-2*v, 1.f);
            unit = glm::vec3(tex_width_unit, -tex_height_unit, 0.f);
            break;
        case 5:
            dir = glm::vec3(1.f-2*u, 1.f-2*v, -1.f);
            unit = glm::vec3(-tex_width_unit, -tex_height_unit, 0.f);
            break;
        default:
            assert(false && "Face num cannot be other number!");
            break;
        }
        return std::tuple(dir, unit);
    }

    float getBasis(const glm::vec3& world_vec, int l, int m) {
        assert(abs(m) <= l);
        auto dir = glm::normalize(world_vec);
        const float inv_sqrt_pi = 0.564190f;
        float x = dir.x, y = dir.y, z = dir.z;
        if (l == 0) {
            return 0.5 * inv_sqrt_pi;
        } else if (l == 1) {
            switch (m) {
            case 0:     return 0.5 * std::sqrt(3) * inv_sqrt_pi * z;
            case 1:     return 0.5 * std::sqrt(3) * inv_sqrt_pi * x;
            case -1:    return 0.5 * std::sqrt(3) * inv_sqrt_pi * y;
            }
        } else if (l == 2) {
            switch (m)
            {
            case 0:     return 0.25 * std::sqrt(5) * inv_sqrt_pi * (2*z*z-x*x-y*y);
            case 1:     return 0.5 * std::sqrt(15) * inv_sqrt_pi * (z*x);
            case -1:    return 0.5 * std::sqrt(15) * inv_sqrt_pi * (y*z);
            case 2:     return 0.25 * std::sqrt(15) * inv_sqrt_pi * (x*x-y*y);
            case -2:    return 0.5 * std::sqrt(15) * inv_sqrt_pi * (x*y);
            }
        } else if (l == 3) {
            switch (m) {
            case 0:     return 0.25 * std::sqrt(7) * inv_sqrt_pi * z*(2*z*z-3*x*x-3*y*y);
            case 1:     return 0.25 * std::sqrt(21.0/2.0) * inv_sqrt_pi * x*(5*z*z-1);
            case -1:    return 0.25 * std::sqrt(21.0/2.0) * inv_sqrt_pi * y*(5*z*z-1);
            case 2:     return 0.25 * std::sqrt(105) * inv_sqrt_pi * z*(x*x-y*y);
            case -2:    return 0.5 * std::sqrt(105) * inv_sqrt_pi * x*y*z;
            case 3:     return 0.25 * std::sqrt(35.0/2.0) * inv_sqrt_pi * x*(x*x-3*y*y);
            case -3:    return 0.25 * std::sqrt(35.0/2.0) * inv_sqrt_pi * y*(3*x*x-y*y);
            }
        } else {
            assert(false && "sh degree >= 4");
        }
    }

    float toneMap(float v) {
        // Reinhard
        return v / (1.0f + v);
    }

    bool saveCubemapFaces(const std::vector<float> &pixels, int faceW,
                          int faceH, const std::string &baseName) {
        const size_t pixelCountPerFace = static_cast<size_t>(faceW) * faceH * 3;
        if (pixels.size() != pixelCountPerFace * 6)
            return false;

        const char *suffix[6] = {"_posx.png", "_negx.png", "_posy.png",
                                 "_negy.png", "_posz.png", "_negz.png"};

        std::vector<unsigned char> tmp(pixelCountPerFace);

        for (int f = 0; f < 6; ++f) {
            const float *src = pixels.data() + f * pixelCountPerFace;
            for (size_t i = 0; i < pixelCountPerFace; ++i) {
                float v = src[i];
                v = (v < 0.0f) ? 0.0f : (v > 1.0f) ? 1.0f : v;
                tmp[i] = static_cast<unsigned char>(v * 255.0f + 0.5f);
            }

            std::string filename = baseName + suffix[f];
            if (!stbi_write_png(filename.c_str(), faceW, faceH, 3, tmp.data(),
                                faceW * 3)) {
                return false;
            }
        }
        return true;
    }
};