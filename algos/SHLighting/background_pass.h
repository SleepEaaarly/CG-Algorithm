#include "render_pass.h"
#include "resource_manager.h"
#include "common_render.h"
#include "texture.h"

class BackgroundPass : public IRenderPass {
  public:
    BackgroundPass() {
        shader = std::make_shared<Shader>("shaders/background.vert", "shaders/background.frag");
    }

    void init() override {
        // set depth function to less than AND equal for skybox depth trick.
        glDepthFunc(GL_LEQUAL);

        shader->use();
        shader->setInt("environmentMap", 0);
        shader->setMat4("projection", ResourceManager::getInstance().getMainCamera().getProjectionMatrix());
    }

    void render() override {
        shader->use();
        shader->setMat4("view", ResourceManager::getInstance().getMainCamera().getViewMatrix());
        glActiveTexture(GL_TEXTURE0);
        auto env_cubemap = ResourceManager::getInstance().getCubemap("env_cubemap");
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->getId());
        renderCube();
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void update() override {}
};