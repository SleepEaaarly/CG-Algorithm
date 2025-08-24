#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cassert>

const GLint Texture2D::getId() { return id; }

Texture2D::Texture2D(int width, int height, GLenum internal_format,
                     GLenum image_format, GLenum data_type, GLuint wrap_s,
                     GLuint wrap_t, GLuint filter_min, GLuint filter_max,
                     Type type)
    : width(width), height(height), internal_format(internal_format),
      image_format(image_format), data_type(data_type), wrap_s(wrap_s),
      wrap_t(wrap_t), filter_min(filter_min), filter_max(filter_max),
      texture_type(type) {}

Texture2D::Texture2D(const std::string &path, Type type, GLenum internal_format,
                     GLenum image_format, GLenum data_type, GLuint wrap_s,
                     GLuint wrap_t, GLuint filter_min, GLuint filter_max)
    : Texture2D(0, 0, internal_format, image_format, data_type, wrap_s, wrap_t, filter_min,
                filter_max, type) {
    this->path = path;
    loadTextureFromFile(path);
}

void Texture2D::loadTextureFromFile(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &id);

    int w, h, channel_num;
    unsigned char *data = stbi_load(path.c_str(), &w, &h, &channel_num, 0);
    if (data) {
        width = static_cast<GLuint>(w);
        height = static_cast<GLuint>(h);
        if (channel_num == 1)
            image_format = GL_RED;
        else if (channel_num == 3)
            image_format = GL_RGB;
        else if (channel_num == 4)
            image_format = GL_RGBA;

        internal_format = image_format; // default equal

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0,
                     image_format, data_type, data);
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

const std::string &Texture2D::getPath() { return path; }

const Texture2D::Type Texture2D::getType() { return texture_type; }

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
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format,
                     width, height, 0, image_format, data_type, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_max);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
