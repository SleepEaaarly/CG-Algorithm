#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

class Texture2D;
class Cubemap;

class Framebuffer {
  private:
    unsigned int id;
    unsigned int width, height;
    unsigned int renderbuffer_id;
    GLenum renderbuffer_internal_format;
    GLenum renderbuffer_attachment;

  public:
    Framebuffer(unsigned int width, unsigned int height, GLenum renderbuffer_internal_format = GL_DEPTH_COMPONENT24,
                GLenum renderbuffer_attachment = GL_DEPTH_ATTACHMENT);
    ~Framebuffer();

    void setColorAttachment(std::shared_ptr<Texture2D> texture);
    void setColorAttachment(std::vector<std::shared_ptr<Texture2D>>& texture);
    void setColorAttachment(std::shared_ptr<Cubemap> texture, unsigned int face, unsigned int mip = 0);

    unsigned int getId() const { return id; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

    void resize(unsigned int new_width, unsigned int new_height);
};