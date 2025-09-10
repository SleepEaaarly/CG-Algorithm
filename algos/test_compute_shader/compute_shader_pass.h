#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "app.h"

class ComputeShaderPass : public IRenderPass {
public:
    ComputeShaderPass() {
        shader = std::make_shared<Shader>("algos/test_compute_shader/comp.comp");
    }

    virtual void init() override {
        ResourceManager::getInstance().registerTexture2D("comp_texture", 
            std::make_shared<Texture2D>(1000, 1000, Texture2D::Type::None, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
        auto tex = ResourceManager::getInstance().getTexture2D("comp_texture");
        glBindImageTexture(0, tex->getId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    }

    virtual void update() override {

    }

    virtual void render() override {
        shader->use();
        shader->setFloat("t", App::getInstance().getCurrentTime());
        auto tex = ResourceManager::getInstance().getTexture2D("comp_texture");
        glDispatchCompute(tex->getWidth()/10, tex->getHeight()/10, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
};