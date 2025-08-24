#pragma once

#include "model.h"

class IGameObject {
protected:
    Model model;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 model_matrix;
    glm::mat3 normal_matrix;

public:
    IGameObject(Model::Type type, const glm::vec3& pos = glm::vec3(0.0f), 
               const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scl = glm::vec3(1.0f))
        : model(type), position(pos), rotation(rot), scale(scl) {
        updateMatrix();
    }

    IGameObject(const std::string& model_path, const glm::vec3& pos = glm::vec3(0.0f), 
               const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scl = glm::vec3(1.0f))
        : model(model_path), position(pos), rotation(rot), scale(scl) {
        updateMatrix();
    }
    
    virtual ~IGameObject() = default;

    virtual void init() = 0;

    void setPosition(const glm::vec3& pos) {
        position = pos;
        updateMatrix();
    }

    void setRotation(const glm::vec3& rot) {
        rotation = rot;
        updateMatrix();
    }

    void setScale(const glm::vec3& scl) {
        scale = scl;
        updateMatrix();
    }

    void update() {
        // Update logic for the game object can be added here
        // For now, we just update the model matrix
        updateMatrix();
    }

    void updateMatrix() {
        model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, position);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model_matrix = glm::scale(model_matrix, scale);
        normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
    }

    const glm::mat4& getModelMatrix() const {
        return model_matrix;
    }

    const glm::mat3& getNormalMatrix() const {
        return normal_matrix;
    }

    void draw(Shader& shader) {
        shader.setMat4("model", model_matrix);
        shader.setMat3("normal_matrix", normal_matrix);
        model.draw(shader);
    }

};