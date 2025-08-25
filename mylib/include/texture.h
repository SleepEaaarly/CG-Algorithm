#pragma once

#include <glad/glad.h>
#include <string>

class Texture2D {
  public:
    enum Type { Albedo, Metallic, Normal, Roughness, AO, None };

  private:
    GLuint id;
    GLuint width, height;
    GLenum internal_format, image_format, data_type;
    GLuint wrap_s, wrap_t;
    GLuint filter_min, filter_max;
    Type texture_type;
    std::string path;

  public:
    Texture2D(unsigned int width, unsigned int height, Type type,
              GLenum internal_format, GLenum image_format,
              GLenum data_type, GLuint wrap_s,
              GLuint wrap_t,
              GLuint filter_min,
              GLuint filter_max);
    
    Texture2D(const std::string &path, Type type);

    Texture2D(const std::string &path, Type type,
              GLenum internal_format, GLenum image_format,
              GLenum data_type, GLuint wrap_s,
              GLuint wrap_t,
              GLuint filter_min,
              GLuint filter_max);

    ~Texture2D() = default;

    // load texture with stbi_image.h and modify some params according to images
    void loadUnsignedIntTextureFromFile(const std::string &path, bool change_format);

    void loadFloatTextureFromFile(const std::string &path);

    const std::string &getPath() { return path; }

    const Type getType() { return texture_type; }

    const GLint getId() { return id; }

    const unsigned int getWidth() { return width; }
    const unsigned int getHeight() { return height; }
};

class Cubemap {
  private:
    GLuint id;
    GLuint width, height;
    GLenum internal_format, image_format, data_type;
    GLuint wrap_s, wrap_t, wrap_r;
    GLuint filter_min, filter_max;

  public:
    Cubemap(GLuint width, GLuint height, GLenum internal_format = GL_RGB16F,
            GLenum image_format = GL_RGB, GLenum data_type = GL_FLOAT,
            GLuint wrap_s = GL_CLAMP_TO_EDGE, GLuint wrap_t = GL_CLAMP_TO_EDGE,
            GLuint wrap_r = GL_CLAMP_TO_EDGE,
            GLuint filter_min = GL_LINEAR_MIPMAP_LINEAR,
            GLuint filter_max = GL_LINEAR);

    ~Cubemap() = default;

    const GLint getId() { return id; }

    const unsigned int getWidth() { return width; }
    const unsigned int getHeight() { return height; }
};
