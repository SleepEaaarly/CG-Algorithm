#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "framebuffer.h"
#include "common_render.h"
#include <iostream>

class HDRMap2CubemapPass : public IRenderPass {
  public:
    HDRMap2CubemapPass() {
        shader = std::make_shared<Shader>("algos/ibl/cubemap.vert", "algos/ibl/equirectangular_to_cubemap.frag");
    }

    void init() override {
        // set depth function to less than AND equal for skybox depth trick.
        glDepthFunc(GL_LEQUAL);
        // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    void render() override {
        ResourceManager::getInstance().registerTexture2D("hdr_texture", std::make_shared<Texture2D>("textures/newport_loft.hdr", Texture2D::Type::None, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
        ResourceManager::getInstance().registerCubemap("env_cubemap", std::make_shared<Cubemap>(512, 512, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR));
        ResourceManager::getInstance().registerFramebuffer("capture_fbo", std::make_shared<Framebuffer>(512, 512));
        auto hdr_texture = ResourceManager::getInstance().getTexture2D("hdr_texture");
        auto env_cubemap = ResourceManager::getInstance().getCubemap("env_cubemap");
        auto capture_fbo = ResourceManager::getInstance().getFramebuffer("capture_fbo");
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
        shader->setInt("equirectangularMap", 0);
        shader->setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdr_texture->getId());
        glViewport(0, 0, env_cubemap->getWidth(), env_cubemap->getHeight()); // don't forget to configure the viewport to the capture dimensions.
        for (unsigned int i = 0; i < 6; ++i) {
            shader->setMat4("view", captureViews[i]);
            capture_fbo->setColorAttachment(env_cubemap, i);
            glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->getId());
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }   

    void update() override {}
};
