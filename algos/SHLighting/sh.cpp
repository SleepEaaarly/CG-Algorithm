#include "sh_sampler.h"
#include "app.h"
#include "hdrmap2cubemap_pass.h"
#include "background_pass.h"

int main() {
    App::getInstance().init(1280, 720, "SH-Lighting");

    ResourceManager::getInstance().registerPreRenderPass("hdr2cubemap_pass", std::make_shared<HDRMap2CubemapPass>());
    ResourceManager::getInstance().registerRenderPass("background_pass", std::make_shared<BackgroundPass>());
    
    App::getInstance().initResource();
    App::getInstance().runPreRender();
    SHSampler sampler(3, "sh_coffs.txt");
    if (!sampler.hasData()) {
        sampler.calcSHCoeffs();
    }
}