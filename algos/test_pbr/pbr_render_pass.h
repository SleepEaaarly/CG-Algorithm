#pragma once

#include "render_pass.h"
#include "game_object.h"
#include "resource_manager.h"
#include "light.h"
#include "app.h"
#include <vector>
#include "util.h"

class PBRRenderPass : public IRenderPass {
private:
    std::shared_ptr<IGameObject> obj;
    std::vector<Light> lights;

public:
    PBRRenderPass(std::shared_ptr<IGameObject> object) : obj(object) {
        shader = std::make_shared<Shader>("algos/test_pbr/pbr.vert", "algos/test_pbr/pbr.frag");
    }   

    void init() override {
        lights.push_back(Light(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(200.0f, 200.0f, 200.0f)));
        lights.push_back(Light(glm::vec3(0.0f, 0.f, 5.f), glm::vec3(200.0f, 200.0f, 200.0f)));
        lights.push_back(Light(glm::vec3(0.0f, 0.f, -5.f), glm::vec3(200.0f, 200.0f, 200.0f)));
        lights.push_back(Light(glm::vec3(0.0f, 0.f, -5.f), glm::vec3(200.0f, 200.0f, 200.0f)));
    }

    void update() override {
        shader->use();
        shader->setMat4("model", obj->getModelMatrix());
        shader->setMat3("normal_matrix", obj->getNormalMatrix());
        shader->setVec3("viewPos", ResourceManager::getInstance().getMainCamera().getPosition());
        shader->setMat4("projection", ResourceManager::getInstance().getMainCamera().GetProjectionMatrix());
        shader->setMat4("view", ResourceManager::getInstance().getMainCamera().GetViewMatrix());
        for (int i = 0; i < 4; ++i) {
            float t = App::getInstance().getCurrentTime();
            if (t > 4.f)    t -= 4.f;
            float phase_deg = (90.f * t + ((i%2==0) ? 0.f : 180.f));
            glm::vec3 rotate = glm::vec3(10.f * cos(deg2rad(phase_deg)), 10.f * sin(deg2rad(phase_deg)), 0.f);
            glm::vec3 newPos = lights[i].getPosition() + rotate;
            shader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            shader->setVec3("lightColors[" + std::to_string(i) + "]", lights[i].getColor());
        }
    }

    void render() override {
        obj->draw(*shader);
    }
};