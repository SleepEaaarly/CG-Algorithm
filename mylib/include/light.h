#pragma once

#include <glm/glm.hpp>

class Light {
private:
    glm::vec3 color;

public:
    Light(const glm::vec3& col) : color(col) {}

    virtual const glm::vec3& getColor() const {
        return color;
    }

    virtual void setColor(const glm::vec3& col) {
        color = col;
    }

};

class PointLight : public Light {
private:
    glm::vec3 position;

public:
    PointLight(const glm::vec3& pos, const glm::vec3& color) : 
    Light(color), position(pos) {}
    
    const glm::vec3& getPosition() const {
        return position;
    }
    
    void setPosition(const glm::vec3& pos) {
        this->position = pos;
    }

};