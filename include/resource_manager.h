#pragma once

#include "singleton.h"
#include "game_object.h"
#include "render_pass.h"

#include <vector>
#include <memory>
#include <unordered_map>


class ResourceManager : public Singleton<ResourceManager> {
    friend class Singleton<ResourceManager>;

public:
    const Camera& getMainCamera() const {
        return main_camera;
    }

    void registerGameObject(const std::string& name, const IGameObject& obj) {
        gameObjects_map[name] = gameObjects.size();
        gameObjects.push_back(obj);
    }

    void registerRenderPass(const std::string& name, const IRenderPass& pass) {
        renderPasses.push_back(pass);
    }

    void updateGameObjects() {
        for (auto& obj : gameObjects) {
            obj.update();
        }
    }

    void updateRenderPasses() {
        for (auto& pass : renderPasses) {
            pass.update();
        }
    }

    IGameObject& getGameObject(const std::string& name) {
        auto it = gameObjects_map.find(name);
        if (it == gameObjects_map.end()) {
            throw std::out_of_range("GameObject not found: " + name);
        }
        return gameObjects[it->second];
    }

    void render() {
        for (auto& pass : renderPasses) {
            pass.render();
        }
    }

private:
    std::vector<IGameObject> gameObjects;
    std::vector<IRenderPass> renderPasses;
    std::unordered_map<std::string, size_t> gameObjects_map;
    Camera main_camera{glm::vec3(0.0f, 0.0f, 3.0f)};

    ResourceManager() = default;
    ~ResourceManager() = default;

};
