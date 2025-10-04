#pragma once

#include "singleton.h"
#include "my_camera.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class GameObject;
class IRenderPass;
class Light;
class Texture2D;
class Framebuffer;
class Cubemap;

class ResourceManager : public Singleton<ResourceManager> {
    friend class Singleton<ResourceManager>;

public:
    Camera& getMainCamera();

    void registerGameObject(const std::string& name, std::shared_ptr<GameObject> obj);

    void registerSphere();

    void setupMainCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH, float near = 0.1f, float far = 100.f);

    // Register a render pass that will be executed before the main render pass
    // This is useful for passes that need to set up the scene before the main rendering.
    void registerPreRenderPass(const std::string& name, std::shared_ptr<IRenderPass> pass);

    // Must register render pass in the order of rendering
    // e.g. skybox pass first, then PBR pass, etc.
    // This is to ensure that the render passes are executed in the correct order.
    void registerRenderPass(const std::string& name, std::shared_ptr<IRenderPass> pass);
    
    void registerTexture2D(const std::string& name, std::shared_ptr<Texture2D> texture);

    void registerFramebuffer(const std::string& name, std::shared_ptr<Framebuffer> framebuffer);

    void registerCubemap(const std::string& name, std::shared_ptr<Cubemap> cubemap);

    void registerLight(const std::string& name, std::shared_ptr<Light> light);

    void initResources();

    std::vector<std::shared_ptr<GameObject>>& getAllGameObjects();

    std::vector<std::shared_ptr<IRenderPass>>& getAllPreRenderPasses();

    std::vector<std::shared_ptr<IRenderPass>>& getAllRenderPasses();

    std::shared_ptr<GameObject> getGameObject(const std::string& name);

    std::shared_ptr<Texture2D> getTexture2D(const std::string& name);

    std::shared_ptr<Framebuffer> getFramebuffer(const std::string& name);

    std::shared_ptr<Cubemap> getCubemap(const std::string& name);

    std::shared_ptr<Light> getLight(const std::string& name);

private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<IRenderPass>> preRenderPasses;
    std::vector<std::shared_ptr<IRenderPass>> renderPasses;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjects_map;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> texture2ds;
    std::unordered_map<std::string, std::shared_ptr<Framebuffer>> framebuffers;
    std::unordered_map<std::string, std::shared_ptr<Cubemap>> cubemaps;
    std::unordered_map<std::string, std::shared_ptr<Light>> lights;

    Camera main_camera = glm::vec3(0.0f, 0.0f, -3.0f);

    ResourceManager() = default;
    ~ResourceManager() = default;

};
