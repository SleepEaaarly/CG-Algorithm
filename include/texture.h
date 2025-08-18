#pragma once

#include <string>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

class Texture {
public:
    enum Type {
        Albedo,
        Metallic,
        Normal, 
        Roughness,
        None
    };
private:
    GLuint id;
    GLuint width, height;
    GLenum internal_format, image_format;
    GLuint wrap_s, wrap_t;
    GLuint filter_min, filter_max;
    Type texture_type;
    std::string path;
public:

    Texture() : width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB),
    wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR), texture_type(None) {}

    Texture(const std::string& path, Type type) : Texture() {
        this->path = path;
        texture_type = type;
        loadTextureFromFile(path);
    }

    // load texture with stbi_image.h and modify some params according to images
    void loadTextureFromFile(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &id);

        int w, h, channel_num;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &channel_num, 0);
        if (data) {
            width = static_cast<GLuint>(w); height = static_cast<GLuint>(h);
            if (channel_num == 1)
                image_format = GL_RED;
            else if (channel_num == 3)
                image_format = GL_RGB;
            else if (channel_num == 4)
                image_format = GL_RGBA;

            internal_format = image_format;     // default equal

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

            stbi_image_free(data);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            assert(data != nullptr);
            stbi_image_free(data);
        }
    }

    const std::string& getPath() {
        return path;
    }

    const Type& getType() {
        return texture_type;
    }

    const GLint getId() {
        return id;
    }

    ~Texture() {}
};
