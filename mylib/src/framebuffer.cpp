#include "framebuffer.h"
#include "texture.h"
#include <iostream>
#include <cassert>

Framebuffer::Framebuffer(bool use_renderbuffer, GLenum renderbuffer_internal_format, GLenum renderbuffer_attachment)
    : width(0), height(0), use_renderbuffer(use_renderbuffer), renderbuffer_internal_format(renderbuffer_internal_format),
      renderbuffer_attachment(renderbuffer_attachment), depth_tex_id(0) {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    if (use_renderbuffer) {    
        // Create a renderbuffer object for depth and stencil attachment
        glGenRenderbuffers(1, &renderbuffer_id);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
    }

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

    if (use_renderbuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, renderbuffer_internal_format, new_width, new_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    } else {
        if (depth_tex_id != 0)
            assert("You are using texture as depth attachment and it has been initialized. However, your color attachments' size DONT matches.");
    }

    width = new_width;
    height = new_height;
}

void Framebuffer::setColorAttachment(std::shared_ptr<Texture2D> texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (texture->getWidth() != width || texture->getHeight() != height) {
        resize(texture->getWidth(), texture->getHeight());
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
    if (texture.size() == 0)    return;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (texture[0]->getWidth() != width || texture[0]->getHeight() != height) {
        resize(texture[0]->getWidth(), texture[0]->getHeight());
    }
    std::vector<GLenum> draw_buffers(texture.size());
    for (int i = 0; i < texture.size(); ++i) {
        if (texture[i]->getWidth() != width || texture[i]->getHeight() != height) {
            std::cerr << "Error: Texture index " << i << " size does not match first texture size."
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

    auto tex_width = static_cast<unsigned int>(std::pow(0.5, mip) * texture->getWidth());
    auto tex_height = static_cast<unsigned int>(std::pow(0.5, mip) * texture->getHeight());
    if (tex_width != width || tex_height != height) {
        resize(tex_width, tex_height);
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

void Framebuffer::setDepthTexture(std::shared_ptr<Texture2D> depth_texture) {
    depth_tex_id = depth_texture->getId();
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex_id, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
