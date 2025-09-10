#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "common_render.h"

class RenderQuadPass : public IRenderPass {
public:
    RenderQuadPass() {
        shader = std::make_shared<Shader>("algos/test_compute_shader/quad.vert", "algos/test_compute_shader/quad.frag");
    }

    virtual void init() override {
        shader->use();
        shader->setInt("tex", 0);
    }

    virtual void update() override {

    }

    virtual void render() override {
        shader->use();
        auto tex = ResourceManager::getInstance().getTexture2D("comp_texture");
        glBindTexture(GL_TEXTURE_2D, tex->getId());
        glActiveTexture(GL_TEXTURE0);
        renderQuad();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};