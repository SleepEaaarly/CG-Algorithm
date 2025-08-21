#include "resource_manager.h"
#include "camera.h"
#include "game_object.h"
#include "light.h"
#include "render_pass.h"

Camera &ResourceManager::getMainCamera() { return main_camera; }

void ResourceManager::registerGameObject(const std::string &name,
                                         std::shared_ptr<IGameObject> obj) {
    gameObjects_map[name] = obj;
    gameObjects.push_back(obj);
}

void ResourceManager::registerRenderPass(const std::string &name,
                                         std::shared_ptr<IRenderPass> pass) {
    renderPasses.push_back(pass);
}

void ResourceManager::initResources() {
    for (auto obj : gameObjects) {
        obj->init();
    }
    for (auto pass : renderPasses) {
        pass->init();
    }
}

std::vector<std::shared_ptr<IGameObject>> &ResourceManager::getAllGameObjects() {
    return gameObjects;
}

std::vector<std::shared_ptr<IRenderPass>> &ResourceManager::getAllRenderPasses() {
    return renderPasses;
}

std::shared_ptr<IGameObject> ResourceManager::getGameObject(const std::string &name) {
    auto it = gameObjects_map.find(name);
    if (it == gameObjects_map.end()) {
        throw std::out_of_range("GameObject not found: " + name);
    }
    return it->second;
}

void ResourceManager::render() {
    for (auto pass : renderPasses) {
        pass->render();
    }
}
