#include "resource_manager.h"
#include "camera.h"
#include "game_object.h"
#include "light.h"
#include "render_pass.h"
#include <stdexcept>
#include <regex>
#include <iostream>

Camera &ResourceManager::getMainCamera() { return main_camera; }

void ResourceManager::registerGameObject(const std::string &name,
                                         std::shared_ptr<GameObject> obj) {
    static const std::regex re(R"(^Sphere\d*$)");
    if (std::regex_match(name, re)) {
        throw std::runtime_error("Cannot use reserved name: Sphere or SphereN");
    }
    
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

void ResourceManager::registerTexture2D(const std::string &name,
                                        std::shared_ptr<Texture2D> texture) {
    texture2ds[name] = texture;
}

void ResourceManager::registerCubemap(const std::string &name, std::shared_ptr<Cubemap> cubemap) {
    cubemaps[name] = cubemap;
}

void ResourceManager::registerFramebuffer(const std::string &name,
                                           std::shared_ptr<Framebuffer> framebuffer) {
    framebuffers[name] = framebuffer;
}

void ResourceManager::initResources() {
    for (auto obj : gameObjects) {
        obj->init();
    }
    for (auto pass : preRenderPasses) {
        pass->init();
    }
    for (auto pass : renderPasses) {
        pass->init();
    }
}

std::vector<std::shared_ptr<IRenderPass>> &
ResourceManager::getAllPreRenderPasses() {
    return preRenderPasses;
}

std::vector<std::shared_ptr<GameObject>> &
ResourceManager::getAllGameObjects() {
    return gameObjects;
}

std::vector<std::shared_ptr<IRenderPass>> &
ResourceManager::getAllRenderPasses() {
    return renderPasses;
}

std::shared_ptr<GameObject>
ResourceManager::getGameObject(const std::string &name) {
    auto it = gameObjects_map.find(name);
    if (it == gameObjects_map.end()) {
        throw std::out_of_range("GameObject not found: " + name);
        return nullptr; // This line is unreachable but added to avoid compiler
                        // warning
    }
    return it->second;
}

void ResourceManager::registerSphere() {
    static unsigned int sphere = 0;
    sphere++;
    std::string name = "Sphere" + std::to_string(sphere);
    gameObjects_map[name] = std::make_shared<GameObject>(Model::SPHERE);
    gameObjects.push_back(gameObjects_map[name]);
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

std::shared_ptr<Cubemap>
ResourceManager::getCubemap(const std::string &name) {
    auto it = cubemaps.find(name);
    if (it != cubemaps.end()) {
        return it->second;
    }
    throw std::out_of_range("Cubemap not found: " + name);
}