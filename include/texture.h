#pragma once

#include <string>
#include <glad/glad.h>

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

    Texture();

    Texture(const std::string& path, Type type);

    ~Texture() = default;

    // load texture with stbi_image.h and modify some params according to images
    void loadTextureFromFile(const std::string& path);

    const std::string& getPath();

    const Type getType();

    const GLint getId();
};
