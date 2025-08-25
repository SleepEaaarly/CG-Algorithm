#include "render_pass.h"
#include "framebuffer.h"
#include "texture.h"
#include "resource_manager.h"
#include "common_render.h"
#include <iostream>

class BRDFLUTPass : public IRenderPass {
  public:
    BRDFLUTPass() {
        shader = std::make_shared<Shader>("algos/ibl/brdflut.vert", "algos/ibl/brdflut.frag");
    }

    void init() override {
    }

    void render() override {
        ResourceManager::getInstance().registerTexture2D("brdf_lut_texture", std::make_shared<Texture2D>(512, 512, Texture2D::Type::None, GL_RG16F, GL_RG, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
    
        auto brdf_lut_texture = ResourceManager::getInstance().getTexture2D("brdf_lut_texture");
        auto capture_fbo = ResourceManager::getInstance().getFramebuffer("capture_fbo");
        capture_fbo->resize(512, 512);

        capture_fbo->setColorAttachment(brdf_lut_texture);

        glViewport(0, 0, brdf_lut_texture->getWidth(), brdf_lut_texture->getHeight());
        shader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void update() override {}
};