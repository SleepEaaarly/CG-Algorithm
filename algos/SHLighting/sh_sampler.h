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
        auto n = degree * degree;
        std::string line;
        if (!std::getline(fin, line)) {
            return false;
        }
        std::istringstream iss(line);
        sh_coeffs.reserve(n);
        float value;
        size_t cnt = 0;
        while (iss >> value) {
            cnt++;
            if (cnt > n)
                return false;
            sh_coeffs.push_back(value);
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
        for (int face = 0; face < 6; ++face) {
            for (auto row = 0; row < height; ++row) {
                for (auto col = 0; col < width; ++col) {
                    auto ind =
                        (col + width * row + width * height * face) * com;
                    glm::vec3 color(image_data[ind], image_data[ind + 1],
                                    image_data[ind + 2]);
                    glm::vec3 world_vec = getTexelWorldVector(face, row, col, height, width);
                }
            }
        }
    }

    glm::vec3 getTexelWorldVector(int face, unsigned int row, unsigned int col, unsigned int height, unsigned int width) {
        glm::vec3 dir;
        float tex_height_unit = 1.f / height;
        float tex_width_unit = 1.f / width;
        float u = (static_cast<float>(col) + 0.5f) * tex_width_unit;
        float v = (static_cast<float>(row) + 0.5f) * tex_height_unit;
        switch (face) {
        case 0:
            dir = glm::vec3(1.f, 1.f-2*u, 1.f-2*v);
            break;
        case 1:
            dir = glm::vec3(-1.f, 2*u-1.f, 1.f-2*v);
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