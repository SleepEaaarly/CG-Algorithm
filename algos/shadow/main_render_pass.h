#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "render_scene.h"
#include "light.h"

#include <iostream>

class RenderPass : public IRenderPass {
public:
    std::shared_ptr<Texture2D> wood_texture;
    std::shared_ptr<Texture2D> shadow_map;

    RenderPass() {
        shader = std::make_shared<Shader>("algos/shadow/shadow_render.vert", "algos/shadow/shadow_render.frag");
        wood_texture = std::make_shared<Texture2D>("textures/wood.png", Texture2D::Type::None);
    }

    void init() override {
        auto light = std::dynamic_pointer_cast<PointLight>(ResourceManager::getInstance().getLight("light"));
        shader->use();
        shader->setInt("shadow_map", 0);
        shader->setInt("wood_texture", 1);
        shader->setVec3("light_pos", light->getPosition());
        shader->setVec3("light_color", light->getColor());
        auto light_view = glm::lookAt(light->getPosition(), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
        auto light_ortho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 7.5f);
        auto light_space_matrix = light_ortho * light_view;
        shader->setMat4("light_space_matrix", light_space_matrix);
        shadow_map = ResourceManager::getInstance().getTexture2D("shadow_map");
    }

    void render() override {
        auto main_camera = ResourceManager::getInstance().getMainCamera();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_map->getId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wood_texture->getId());
        shader->use();
        shader->setMat4("view", main_camera.getViewMatrix());
        shader->setMat4("projection", main_camera.getProjectionMatrix());
        shader->setVec3("eye_pos", main_camera.getPosition());
        renderScene(shader);
    }
};