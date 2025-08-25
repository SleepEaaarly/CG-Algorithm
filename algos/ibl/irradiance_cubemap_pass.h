#include "render_pass.h"
#include "framebuffer.h"
#include "texture.h"
#include "resource_manager.h"
#include "common_render.h"

#include <iostream>

class IrradianceCubemapPass : public IRenderPass {
  public:
    IrradianceCubemapPass() {
        shader = std::make_shared<Shader>("algos/ibl/cubemap.vert", "algos/ibl/irradiance_convolution.frag");
    }

    void init() override {
    }

    void render() override {
        ResourceManager::getInstance().registerCubemap("irradiance_cubemap", std::make_shared<Cubemap>(32, 32, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
    
        auto env_cubemap = ResourceManager::getInstance().getCubemap("env_cubemap");
        auto irradiance_cubemap = ResourceManager::getInstance().getCubemap("irradiance_cubemap");
        auto capture_fbo = ResourceManager::getInstance().getFramebuffer("capture_fbo");
        capture_fbo->resize(32, 32);
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
        shader->use();
        shader->setInt("environmentMap", 0);
        shader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->getId());
        glViewport(0, 0, irradiance_cubemap->getWidth(), irradiance_cubemap->getHeight()); // don't forget to configure the viewport to the capture dimensions.
        for (unsigned int i = 0; i < 6; ++i) {
            shader->setMat4("view", captureViews[i]);
            capture_fbo->setColorAttachment(irradiance_cubemap, i);
            glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    void update() override {}
};