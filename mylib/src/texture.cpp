#include "texture.h"
#include <stb_image.h>
#include <cassert>
#include <iostream>

Texture2D::Texture2D(unsigned int width, unsigned int height, Type type,
                     GLenum internal_format, GLenum image_format,
                     GLenum data_type, GLuint wrap_s, GLuint wrap_t,
                     GLuint filter_min, GLuint filter_max)
    : id(0), width(width), height(height), internal_format(internal_format),
      image_format(image_format), data_type(data_type), wrap_s(wrap_s),
      wrap_t(wrap_t), filter_min(filter_min), filter_max(filter_max),
      texture_type(type), path("") {
    allocateTextureSpace();
}

Texture2D::Texture2D(const std::string &path, Type type, GLenum internal_format,
                     GLenum image_format, GLenum data_type, GLuint wrap_s,
                     GLuint wrap_t, GLuint filter_min, GLuint filter_max)
    : id(0), width(0), height(0), internal_format(internal_format),
      image_format(image_format), data_type(data_type), wrap_s(wrap_s),
      wrap_t(wrap_t), filter_min(filter_min), filter_max(filter_max),
      texture_type(type), path(path) {
    if (data_type == GL_FLOAT)
        loadFloatTextureFromFile(path);
    else if (data_type == GL_UNSIGNED_BYTE)
        loadUnsignedIntTextureFromFile(path, false);
    else {
        assert(false && "Texture2D Constructor::Not supported data type");
    }
}

Texture2D::Texture2D(const std::string &path, Type type)
    : id(0), width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB),
      data_type(GL_UNSIGNED_BYTE), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
      filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR),
      texture_type(type), path(path) {
    loadUnsignedIntTextureFromFile(path, true);
}

void Texture2D::allocateTextureSpace() {
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0,
                 image_format, data_type, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::loadFloatTextureFromFile(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &id);

    int w, h, channel_num;
    float *data = stbi_loadf(path.c_str(), &w, &h, &channel_num, 0);
    if (data) {
        width = static_cast<GLuint>(w);
        height = static_cast<GLuint>(h);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0,
                     image_format, data_type, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        assert(data != nullptr && "Failed to load texture");
        stbi_image_free(data);
    }
}

void Texture2D::loadUnsignedIntTextureFromFile(const std::string &path,
                                               bool change_format) {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &id);

    int w, h, channel_num;
    unsigned char *data = stbi_load(path.c_str(), &w, &h, &channel_num, 0);
    if (data) {
        width = static_cast<GLuint>(w);
        height = static_cast<GLuint>(h);
        if (change_format) {
            if (channel_num == 1)
                image_format = GL_RED;
            else if (channel_num == 3)
                image_format = GL_RGB;
            else if (channel_num == 4)
                image_format = GL_RGBA;

            internal_format = image_format; // default equal
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0,
                     image_format, data_type, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        assert(data != nullptr && "Failed to load texture");
        stbi_image_free(data);
    }
}

Cubemap::Cubemap(GLuint width, GLuint height, GLenum internal_format,
                 GLenum image_format, GLenum data_type, GLuint wrap_s,
                 GLuint wrap_t, GLuint wrap_r, GLuint filter_min,
                 GLuint filter_max)
    : width(width), height(height), internal_format(internal_format),
      image_format(image_format), data_type(data_type), wrap_s(wrap_s),
      wrap_t(wrap_t), wrap_r(wrap_r), filter_min(filter_min),
      filter_max(filter_max) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (size_t i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, image_format, data_type, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_max);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

std::vector<float> Cubemap::getData() {
    if (internal_format != GL_RGB16F && internal_format != GL_RGB32F) {
        std::cerr << "Only for Float type cubemap to get level 0 data" << std::endl;
        return {};
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    size_t unit_size = width * height * 3;
    std::vector<float> image_data(6 * unit_size);

    for (size_t i = 0; i < 6; ++i) {
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, image_data.data() + i*unit_size);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return image_data;
}