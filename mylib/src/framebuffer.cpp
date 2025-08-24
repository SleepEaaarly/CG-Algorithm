#include "framebuffer.h"
#include "texture.h"
#include <iostream>
#include <vector>

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

void Framebuffer::addColorAttachment(std::shared_ptr<Texture2D> texture) {
    if (texture->getWidth() != width || texture->getHeight() != height) {
        std::cerr << "Error: Texture size does not match framebuffer size."
                  << std::endl;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + color_attachment_count,
                           GL_TEXTURE_2D, texture->getId(), 0);
    color_attachment_count++;

    // Specify the list of color buffers to be drawn into
    std::vector<GLenum> draw_buffers(color_attachment_count);
    for (unsigned int i = 0; i < color_attachment_count; ++i) {
        draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(color_attachment_count, draw_buffers.data());

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}