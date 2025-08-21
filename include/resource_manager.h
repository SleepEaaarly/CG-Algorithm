#pragma once

#include "singleton.h"
#include "camera.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class IGameObject;
class IRenderPass;
class Light;

class ResourceManager : public Singleton<ResourceManager> {
    friend class Singleton<ResourceManager>;

public:
    Camera& getMainCamera();

    void registerGameObject(const std::string& name, std::shared_ptr<IGameObject> obj);

    void registerRenderPass(const std::string& name, std::shared_ptr<IRenderPass> pass);
    
    void initResources();

    std::vector<std::shared_ptr<IGameObject>>& getAllGameObjects();

    std::vector<std::shared_ptr<IRenderPass>>& getAllRenderPasses();

    std::shared_ptr<IGameObject> getGameObject(const std::string& name);

    void render();

private:
    std::vector<std::shared_ptr<IGameObject>> gameObjects;
    std::vector<std::shared_ptr<IRenderPass>> renderPasses;
    std::unordered_map<std::string, std::shared_ptr<IGameObject>> gameObjects_map;
    Camera main_camera = glm::vec3(0.0f, 0.0f, 3.0f);

    ResourceManager() = default;
    ~ResourceManager() = default;

};
