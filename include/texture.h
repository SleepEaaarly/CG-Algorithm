#pragma once

#include <glad/glad.h>
#include <string>

class Texture2D {
  public:
    enum Type { Albedo, Metallic, Normal, Roughness, None };

  private:
    GLuint id;
    GLuint width, height;
    GLenum internal_format, image_format, data_type;
    GLuint wrap_s, wrap_t;
    GLuint filter_min, filter_max;
    Type texture_type;
    std::string path;

  public:
    Texture2D(int width = 0, int height = 0, GLenum internal_format = GL_RGB,
              GLenum image_format = GL_RGB, GLenum data_type = GL_UNSIGNED_BYTE,
              GLuint wrap_s = GL_REPEAT,
              GLuint wrap_t = GL_REPEAT,
              GLuint filter_min = GL_LINEAR_MIPMAP_LINEAR,
              GLuint filter_max = GL_LINEAR, Type type = None);

    Texture2D(const std::string &path, Type type,
              GLenum internal_format = GL_RGB, GLenum image_format = GL_RGB,
              GLenum data_type = GL_UNSIGNED_BYTE,
              GLuint wrap_s = GL_REPEAT, GLuint wrap_t = GL_REPEAT,
              GLuint filter_min = GL_LINEAR_MIPMAP_LINEAR,
              GLuint filter_max = GL_LINEAR);

    ~Texture2D() = default;

    // load texture with stbi_image.h and modify some params according to images
    void loadTextureFromFile(const std::string &path);

    const std::string &getPath();

    const Type getType();

    const GLint getId();
};

class Cubemap {
  private:
    GLuint id;
    GLuint width, height;
    GLenum internal_format, image_format, data_type;
    GLuint wrap_s, wrap_t, wrap_r;
    GLuint filter_min, filter_max;

  public:
    Cubemap(GLuint width, GLuint height,
            GLenum internal_format = GL_RGB16F, GLenum image_format = GL_RGB,
            GLenum data_type = GL_FLOAT,
            GLuint wrap_s = GL_CLAMP_TO_EDGE,
            GLuint wrap_t = GL_CLAMP_TO_EDGE, GLuint wrap_r = GL_CLAMP_TO_EDGE,
            GLuint filter_min = GL_LINEAR_MIPMAP_LINEAR,
            GLuint filter_max = GL_LINEAR);
            
    ~Cubemap() = default;
};
