#include "render_pass.h"
#include "resource_manager.h"
#include "framebuffer.h"
#include "texture.h"
#include "light.h"
#include "render_scene.h"
#include <glm/gtc/matrix_transform.hpp>

class ShadowPass : public IRenderPass {
public:
    std::shared_ptr<PointLight> light;
    std::shared_ptr<Framebuffer> capture_fbo;
    std::shared_ptr<Texture2D> shadow_map;

    ShadowPass() {
        shader = std::make_shared<Shader>("algos/shadow/shadow_map.vert", "algos/shadow/shadow_map.frag");
        glm::vec3 pos(-2.f, 4.f, -1.f);
        light = std::make_shared<PointLight>(pos, glm::vec3(1.f, 1.f, 1.f));
        ResourceManager::getInstance().registerLight("light", light);
    } 

    virtual void init() override {
        // set framebuffer 
        capture_fbo = std::make_shared<Framebuffer>(false);
        ResourceManager::getInstance().registerFramebuffer("capture_fbo", capture_fbo);
        shadow_map = std::make_shared<Texture2D>(1024, 1024, Texture2D::Type::None, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT,
             GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST);
        ResourceManager::getInstance().registerTexture2D("shadow_map", shadow_map);
        glBindTexture(GL_TEXTURE_2D, shadow_map->getId());
        float borderColor[] = {1.f, 1.f, 1.f, 1.f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindTexture(GL_TEXTURE_2D, 0);
        capture_fbo->setDepthTexture(shadow_map);
        glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        #include <iostream>
        // set shader
        shader->use();
        shader->setMat4("view", glm::lookAt(light->getPosition(), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f)));
        shader->setMat4("projection", glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 7.5f));
    }

    virtual void update() override {

    }

    virtual void render() override {
        glViewport(0, 0, shadow_map->getWidth(), shadow_map->getHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
        glClear(GL_DEPTH_BUFFER_BIT);
        shader->use();
        renderScene(shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};