#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_image_write.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "texture.h"
#include "resource_manager.h"

class SHSampler {
  private:
    int degree;
    std::vector<glm::vec3> sh_coeffs;

    std::string txt_path;
    bool has_data;

    bool read_sh_coeffs() {
        std::ifstream fin(txt_path);
        if (!fin) {
            return false;
        }
        std::string header = ResourceManager::getInstance().getTexture2D("hdr_texture")->getPath();

        std::string first_line;
        std::getline(fin, first_line);
        if (first_line != header)   return false;

        auto n = (degree+1) * (degree+1);
        float x, y, z;
        while (fin >> x >> y >> z) {
            sh_coeffs.push_back(glm::vec3(x, y, z));
        }

        return sh_coeffs.size() == n;
    }

    bool save_sh_coeffs() {
        std::ofstream ofs(txt_path);
        if (!ofs) return false;

        std::string header = ResourceManager::getInstance().getTexture2D("hdr_texture")->getPath();
        ofs << header << '\n';

        ofs << std::fixed;
        for (const auto& v : sh_coeffs)
            ofs << v.x << ' ' << v.y << ' ' << v.z << '\n';
        return ofs.good();
    }

  public:
    SHSampler(size_t deg, const char *txt_path, bool load = true) : degree(deg), txt_path(txt_path) {
        if (load)   has_data = read_sh_coeffs();
        else has_data = false;
    }

    bool hasData() { return has_data; }

    void calcSHCoeffs(std::shared_ptr<Cubemap> cubemap, bool save, int com = 3) {
        // com = 3 : rgb
        auto image_data = cubemap->getData();
        unsigned int width = cubemap->getWidth(), height = cubemap->getHeight();
        assert(width == height && "Cubemap Width must equal Height");
        int n = (degree+1)*(degree+1);
        sh_coeffs.assign(n, glm::vec3(0.f));
        for (int face = 0; face < 6; ++face) {
            for (auto row = 0; row < height; ++row) {
                for (auto col = 0; col < width; ++col) {
                    auto ind =
                        (col + width * row + width * height * face) * com;
                    glm::vec3 color(image_data[ind], image_data[ind + 1],
                                    image_data[ind + 2]);
                    auto world_vec = getTexelWorldVector(face, row, col, height, width);
                    auto dw = solidAnglePixel(col, row, width);
                    for (int l = 0; l <= degree; ++l) {
                        for (int m = -l; m <= l; ++m) {
                            int ind = l*(l+1)+m;
                            sh_coeffs[ind] += color * getBasis(world_vec, l, m) * dw;
                        }
                    }

                }
            }
        }
        if (save) {
            save_sh_coeffs();
        }
    }

    float solidAnglePixel(int u, int v, int N) {
        const double du = 2.0 / N;
        double x0 = -1.0 + (u    ) * du;   // 左边界
        double x1 = -1.0 + (u + 1) * du;   // 右边界
        double y0 = -1.0 + (v    ) * du;
        double y1 = -1.0 + (v + 1) * du;

        auto R = [](double x, double y)
        {
            return std::sqrt(x * x + y * y + 1.0);
        };

        return std::atan2(x1 * y1, R(x1, y1))
            - std::atan2(x0 * y1, R(x0, y1))
            - std::atan2(x1 * y0, R(x1, y0))
            + std::atan2(x0 * y0, R(x0, y0));
    }

    glm::vec3 getTexelWorldVector(int face, unsigned int row, unsigned int col, unsigned int height, unsigned int width) {
        glm::vec3 dir;
        float tex_height_unit = 1.f / height;
        float tex_width_unit = 1.f / width;
        float u = (static_cast<float>(col) + 0.5f) * tex_width_unit;
        float v = (static_cast<float>(row) + 0.5f) * tex_height_unit;
        switch (face) {
        case 0:
            dir = glm::vec3(1.f, 1.f-2*v, 1.f-2*u);
            break;
        case 1:
            dir = glm::vec3(-1.f, 1.f-2*v, 2*u-1.f);
            break;
        case 2:
            dir = glm::vec3(2*u-1.f, 1.f, 2*v-1.f);
            break;
        case 3:
            dir = glm::vec3(2*u-1.f, -1.f, 1.f-2*v);
            break;
        case 4:
            dir = glm::vec3(2*u-1.f, 1.f-2*v, 1.f);
            break;
        case 5:
            dir = glm::vec3(1.f-2*u, 1.f-2*v, -1.f);
            break;
        default:
            assert(false && "Face num cannot be other number!");
            break;
        }
        return dir;
    }

    float getBasis(const glm::vec3& world_vec, int l, int m) {
        assert(abs(m) <= l);
        auto dir = glm::normalize(world_vec);
        const float inv_sqrt_pi = 0.564190f;
        float x = dir.x, y = dir.y, z = dir.z;
        if (l == 0) {
            const float c0 = 0.282095f;  // 0.5 * inv_sqrt_pi
            return c0;
        } else if (l == 1) {
            const float c0 = 0.488603f; // 0.5 * std::sqrt(3) * inv_sqrt_pi
            switch (m) {
            case 0:     return c0 * z;
            case 1:     return c0 * x;
            case -1:    return c0 * y;
            }
        } else if (l == 2) {
            const float c0 = 1.092548f; // 0.5 * std::sqrt(15) * inv_sqrt_pi
            const float c1 = 0.315392f; // 0.25 * std::sqrt(5) * inv_sqrt_pi
            const float c2 = 0.546274f; // 0.25 * std::sqrt(15) * inv_sqrt_pi
            switch (m)
            {
            case 0:     return c1 * (2*z*z-x*x-y*y);
            case 1:     return c0 * (z*x);
            case -1:    return c0 * (y*z);
            case 2:     return c2 * (x*x-y*y);
            case -2:    return c0 * (x*y);
            }
        } else if (l == 3) {
            const float c0 = 0.373176;  // 0.25 * std::sqrt(7) * inv_sqrt_pi
            const float c1 = 0.457046;  // 0.25 * std::sqrt(21.0/2.0) * inv_sqrt_pi
            const float c2 = 1.445306;  // 0.25 * std::sqrt(105) * inv_sqrt_pi
            const float c3 = 0.590044;  // 0.25 * std::sqrt(35.0/2.0) * inv_sqrt_pi
            switch (m) {
            case 0:     return c0 * z*(2*z*z-3*x*x-3*y*y);
            case 1:     return c1 * x*(5*z*z-1);
            case -1:    return c1 * y*(5*z*z-1);
            case 2:     return c2 * z*(x*x-y*y);
            case -2:    return 2 * c2 * x*y*z;
            case 3:     return c3 * x*(x*x-3*y*y);
            case -3:    return c3 * y*(3*x*x-y*y);
            }
        } else {
            assert(false && "sh degree >= 4");
        }
    }

    const std::vector<glm::vec3> getSHLightCoeffs() {
        return sh_coeffs;
    } 

    bool saveCubemapFaces(const std::vector<float> &pixels, int faceW,
                          int faceH, const std::string &baseName, bool reverse = true) {
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

            if (reverse) {
                std::vector<unsigned char> row(faceW * 3);
                // 上下翻转
                const size_t stride = static_cast<size_t>(faceW) * 3;
                for (int y = 0; y < faceH / 2; ++y) {
                    unsigned char* top = tmp.data() + y * stride;
                    unsigned char* bot = tmp.data() + (faceH - 1 - y) * stride;

                    std::copy(top, top + stride, row.begin());
                    std::copy(bot, bot + stride, top);
                    std::copy(row.begin(), row.end(), bot);
                }
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