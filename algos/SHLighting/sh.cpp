#include "sh_sampler.h"
#include "app.h"
#include "hdrmap2cubemap_pass.h"
#include "background_pass.h"
#include "sh_lighting_pass.h"
#include "test_pass.h"

int main() {
    bool color_block_test = true;
    App::getInstance().init(1280, 720, "SH-Lighting");

    ResourceManager::getInstance().registerPreRenderPass("hdr2cubemap_pass", std::make_shared<HDRMap2CubemapPass>());
    if (color_block_test)
        ResourceManager::getInstance().registerPreRenderPass("test_pass", std::make_shared<TestPass>());

    App::getInstance().initResource();
    App::getInstance().runPreRender();
    int sh_degree = 3;
    SHSampler sampler(sh_degree, "algos/SHLighting/sh_coffs.txt", !color_block_test);
    if (!sampler.hasData()) {
        auto skybox = ResourceManager::getInstance().getCubemap("env_cubemap");
        sampler.calcSHCoeffs(skybox, !color_block_test);
    }

    ResourceManager::getInstance().registerRenderPass("sh_lighting_pass", std::make_shared<SHLightingPass>(sh_degree, sampler.getSHLightCoeffs()));
    ResourceManager::getInstance().registerRenderPass("background_pass", std::make_shared<BackgroundPass>());

    App::getInstance().initResource();
    App::getInstance().runRenderLoop();
}