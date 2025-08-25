#include "app.h"
#include "resource_manager.h"
#include "hdrmap2cubemap_pass.h"
#include "irradiance_cubemap_pass.h"
#include "prefilter_pass.h"
#include "brdflut_pass.h"
#include "pbr_render_pass.h"
#include "background_pass.h"


int main(int argc, char** argv) {
    App::getInstance().init(800, 600, "IBL");

    ResourceManager::getInstance().registerSphere();
    auto sphere = ResourceManager::getInstance().getGameObject("Sphere1");

    ResourceManager::getInstance().registerPreRenderPass("hdr2cubemap_pass", std::make_shared<HDRMap2CubemapPass>());
    ResourceManager::getInstance().registerPreRenderPass("irradiance_pass", std::make_shared<IrradianceCubemapPass>());
    ResourceManager::getInstance().registerPreRenderPass("prefilter_pass", std::make_shared<PrefilterPass>());
    ResourceManager::getInstance().registerPreRenderPass("brdflut_pass", std::make_shared<BRDFLUTPass>());
    ResourceManager::getInstance().registerRenderPass("pbr_pass", std::make_shared<PBRRenderPass>(sphere));
    ResourceManager::getInstance().registerRenderPass("background_pass", std::make_shared<BackgroundPass>());

    App::getInstance().run();

    return 0;
}

