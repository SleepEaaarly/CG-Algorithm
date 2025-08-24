#pragma once

#include "game_object.h"

class PBRGun : public IGameObject {
public:
    PBRGun(const std::string& model_path, const glm::vec3& pos = glm::vec3(0.0f), 
           const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scl = glm::vec3(1.0f))
        : IGameObject(model_path, pos, rot, scl) {}

    void init() override {
        model.loadTexturesNotMarked("model/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga", Texture2D::Type::Metallic);
        model.loadTexturesNotMarked("model/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga", Texture2D::Type::Normal);
        model.loadTexturesNotMarked("model/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga", Texture2D::Type::Roughness);
        setScale(glm::vec3(0.01f)); // Scale down the model
    }
};