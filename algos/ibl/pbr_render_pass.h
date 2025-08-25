#include "render_pass.h"
#include "texture.h"
#include "resource_manager.h"
#include "game_object.h"

#include <iostream>

class PBRRenderPass : public IRenderPass {
  private:
    std::shared_ptr<GameObject> pbr_object;

  public:
    PBRRenderPass(std::shared_ptr<GameObject> obj) : pbr_object(obj) {
        shader = std::make_shared<Shader>("algos/ibl/pbr.vert", "algos/ibl/pbr.frag");
    }

    void init() override {
        pbr_object->getModel().loadTexturesNotMarked("textures/rusted_iron_ball/albedo.png", Texture2D::Type::Albedo);
        pbr_object->getModel().loadTexturesNotMarked("textures/rusted_iron_ball/metallic.png", Texture2D::Type::Metallic);
        pbr_object->getModel().loadTexturesNotMarked("textures/rusted_iron_ball/normal.png", Texture2D::Type::Normal);
        pbr_object->getModel().loadTexturesNotMarked("textures/rusted_iron_ball/roughness.png", Texture2D::Type::Roughness);
        pbr_object->getModel().loadTexturesNotMarked("textures/rusted_iron_ball/ao.png", Texture2D::Type::AO);
        pbr_object->setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
        shader->use();
        shader->setMat4("projection", ResourceManager::getInstance().getMainCamera().getProjectionMatrix());
        shader->setMat4("model", pbr_object->getModelMatrix());
        shader->setMat3("normalMatrix", pbr_object->getNormalMatrix());
        shader->setInt("irradianceMap", 5);
        shader->setInt("prefilterMap", 6);
        shader->setInt("brdfLUT", 7);
        glm::vec3 lightPositions[] = {
            glm::vec3(-10.0f,  10.0f, 10.0f),
            glm::vec3( 10.0f,  10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3( 10.0f, -10.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)
        };
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
            shader->setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
            shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        }
    }

    void render() override {
        // bind pre-computed IBL data
        pbr_object->draw(*shader);
    }

    void update() override {
        // update view matrix each frame
        shader->use();
        shader->setMat4("view", ResourceManager::getInstance().getMainCamera().getViewMatrix());
        shader->setVec3("camPos", ResourceManager::getInstance().getMainCamera().getPosition());
        unsigned int texture_num = pbr_object->getModel().getTextureCount();
        glActiveTexture(GL_TEXTURE0 + texture_num);
        auto irradiance_cubemap = ResourceManager::getInstance().getCubemap("irradiance_cubemap");
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_cubemap->getId());
        texture_num++;
        glActiveTexture(GL_TEXTURE0 + texture_num);
        auto prefilter_cubemap = ResourceManager::getInstance().getCubemap("prefilter_cubemap");
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_cubemap->getId());
        texture_num++;
        glActiveTexture(GL_TEXTURE0 + texture_num);
        auto brdf_lut_texture = ResourceManager::getInstance().getTexture2D("brdf_lut_texture");
        glBindTexture(GL_TEXTURE_2D, brdf_lut_texture->getId());
    }
};