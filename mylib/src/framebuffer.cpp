#include "framebuffer.h"
#include "texture.h"
#include <iostream>

Framebuffer::Framebuffer(unsigned int width, unsigned int height, 
GLenum renderbuffer_internal_format, GLenum renderbuffer_attachment)
    : width(width), height(height), renderbuffer_internal_format(renderbuffer_internal_format),
      renderbuffer_attachment(renderbuffer_attachment) {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &renderbuffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, renderbuffer_internal_format, width,
                          height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbuffer_attachment,
                              GL_RENDERBUFFER, renderbuffer_id);

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &id);
    glDeleteRenderbuffers(1, &renderbuffer_id);
}

void Framebuffer::resize(unsigned int new_width, unsigned int new_height) {
    if (new_width == width && new_height == height) {
        return; // No need to resize
    }
    width = new_width;
    height = new_height;

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, renderbuffer_internal_format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::setColorAttachment(std::shared_ptr<Texture2D> texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (texture->getWidth() != width || texture->getHeight() != height) {
        std::cerr << "Error: Texture size does not match framebuffer size."
                  << std::endl;
        return;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, texture->getId(), 0);
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::setColorAttachment(std::vector<std::shared_ptr<Texture2D>>& texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    std::vector<GLenum> draw_buffers(texture.size());
    for (int i = 0; i < texture.size(); ++i) {
        if (texture[i]->getWidth() != width || texture[i]->getHeight() != height) {
            std::cerr << "Error: Texture index " << i << " size does not match framebuffer size."
                      << std::endl;
            return;
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, texture[i]->getId(), 0);
        draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(texture.size(), draw_buffers.data());

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::setColorAttachment(std::shared_ptr<Cubemap> texture, unsigned int face, unsigned int mip) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (mip == 0 && (texture->getWidth() != width || texture->getHeight() != height)) {
        std::cerr << "Error: Texture size does not match framebuffer size."
                  << std::endl;
        return;
    }
    if (face >= 6) {
        std::cerr << "Error: Cubemap face index out of range." << std::endl;
        return;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture->getId(), mip);
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
