#include "render_pass.h"
#include "framebuffer.h"
#include "texture.h"
#include "resource_manager.h"
#include "common_render.h"

class PrefilterPass : public IRenderPass {
  public:
    PrefilterPass() {
        shader = std::make_shared<Shader>("algos/ibl/cubemap.vert", "algos/ibl/prefilter.frag");
    }

    void init() override {
    }

    void render() override {
        ResourceManager::getInstance().registerCubemap("prefilter_cubemap", std::make_shared<Cubemap>(128, 128, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR));
        auto prefilter_cubemap = ResourceManager::getInstance().getCubemap("prefilter_cubemap");
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_cubemap->getId());
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        auto env_cubemap = ResourceManager::getInstance().getCubemap("env_cubemap");
        auto capture_fbo = ResourceManager::getInstance().getFramebuffer("capture_fbo");
        capture_fbo->resize(128, 128);
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
        
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            capture_fbo->resize(mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            shader->setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i) {
                shader->setMat4("view", captureViews[i]);
                capture_fbo->setColorAttachment(prefilter_cubemap, i, mip);
                glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo->getId());
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    }

    void update() override {}
};