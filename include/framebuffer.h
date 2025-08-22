#pragma once

#include <glad/glad.h>
#include <memory>

class Texture2D;

class Framebuffer {
  private:
    unsigned int id;
    unsigned int width, height;
    unsigned int renderbuffer_id;
    GLenum renderbuffer_internal_format;
    GLenum renderbuffer_attachment;
    unsigned int color_attachment_count = 0;

  public:
    Framebuffer(unsigned int width, unsigned int height, GLenum renderbuffer_internal_format = GL_DEPTH_COMPONENT24,
                GLenum renderbuffer_attachment = GL_DEPTH_ATTACHMENT);
    ~Framebuffer();

    void addColorAttachment(std::shared_ptr<Texture2D> texture);
    
    unsigned int getId() const { return id; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
};