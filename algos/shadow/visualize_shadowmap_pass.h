#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "common_render.h"

class VisualizePass : public IRenderPass {
public:
    std::shared_ptr<Texture2D> shadow_map;

    VisualizePass() {
        shader = std::make_shared<Shader>("shaders/post_process.vert", "algos/shadow/visualize_shadowmap.frag");
    }

    void init() override {
        shader->use();
        shader->setInt("shadow_map", 0);
        shadow_map = ResourceManager::getInstance().getTexture2D("shadow_map");
    }

    void render() override {
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_map->getId());
        renderQuad();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};