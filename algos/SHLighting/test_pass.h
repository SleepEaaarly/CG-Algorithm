#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "framebuffer.h"
#include "common_render.h"
#include <iostream>

class TestPass : public IRenderPass {
  public:
    TestPass() {
    }

    void init() override {
    }

    void render() override {
        ResourceManager::getInstance().registerTexture2D("hdr_texture", std::make_shared<Texture2D>("textures/daytime.hdr", Texture2D::Type::None, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
        ResourceManager::getInstance().registerCubemap("env_cubemap", std::make_shared<Cubemap>(512, 512, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
        auto env_cubemap = ResourceManager::getInstance().getCubemap("env_cubemap");
        env_cubemap->setWhite(5);
    }   

    void update() override {}
};
