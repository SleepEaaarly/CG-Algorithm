#include "render_pass.h"
#include "resource_manager.h"
#include "game_object.h"


class SHLightingPass : public IRenderPass {
private:
    std::vector<glm::vec3> sh_lighting_coeffs;
    int sh_degree;
    std::shared_ptr<GameObject> sphere;

public:
    SHLightingPass(int deg, const std::vector<glm::vec3>& sh_lighting_coeffs) : sh_degree(deg), sh_lighting_coeffs(sh_lighting_coeffs) {
        shader = std::make_shared<Shader>("algos/SHLighting/sh.vert", "algos/SHLighting/sh.frag");
    }

    void init() override {
        ResourceManager::getInstance().registerSphere();
        sphere = ResourceManager::getInstance().getGameObject("Sphere1");
        shader->use();
        shader->setInt("sh_degree", sh_degree);
        auto coeff_num = (sh_degree+1)*(sh_degree+1);
        for (int i = 0; i < coeff_num; ++i) {
            shader->setVec3("sh_lighting_coeffs[" + std::to_string(i) + "]", sh_lighting_coeffs[i]);
        }
        shader->setMat4("model", sphere->getModelMatrix());
    }
    
    void update() override {}

    void render() override {
        shader->use();
        shader->setMat4("view", ResourceManager::getInstance().getMainCamera().getViewMatrix());
        shader->setMat4("projection", ResourceManager::getInstance().getMainCamera().getProjectionMatrix());
        sphere->draw(*shader);
    }

};