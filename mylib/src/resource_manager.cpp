#include "resource_manager.h"
#include "camera.h"
#include "game_object.h"
#include "light.h"
#include "render_pass.h"
#include <stdexcept>

Camera &ResourceManager::getMainCamera() { return main_camera; }

void ResourceManager::registerGameObject(const std::string &name,
                                         std::shared_ptr<IGameObject> obj) {
    gameObjects_map[name] = obj;
    gameObjects.push_back(obj);
}

void ResourceManager::registerPreRenderPass(const std::string &name,
                                            std::shared_ptr<IRenderPass> pass) {
    preRenderPasses.push_back(pass);
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

std::vector<std::shared_ptr<IRenderPass>> &
ResourceManager::getAllPreRenderPasses() {
    return preRenderPasses;
}

std::vector<std::shared_ptr<IGameObject>> &
ResourceManager::getAllGameObjects() {
    return gameObjects;
}

std::vector<std::shared_ptr<IRenderPass>> &
ResourceManager::getAllRenderPasses() {
    return renderPasses;
}

std::shared_ptr<IGameObject>
ResourceManager::getGameObject(const std::string &name) {
    auto it = gameObjects_map.find(name);
    if (it == gameObjects_map.end()) {
        throw std::out_of_range("GameObject not found: " + name);
        return nullptr; // This line is unreachable but added to avoid compiler
                        // warning
    }
    return it->second;
}

std::shared_ptr<Framebuffer>
ResourceManager::getFramebuffer(const std::string &name) {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        return it->second;
    }
    throw std::out_of_range("Framebuffer not found: " + name);
}

std::shared_ptr<Texture2D>
ResourceManager::getTexture2D(const std::string &name) {
    auto it = texture2ds.find(name);
    if (it != texture2ds.end()) {
        return it->second;
    }
    throw std::out_of_range("Texture2D not found: " + name);
}
