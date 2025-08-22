#include "app.h"
#include "resource_manager.h"
#include "pbr_gun.h"
#include "pbr_render_pass.h"

int main() {
    App::getInstance().init(800, 600, "CG-Algorithm");

    ResourceManager::getInstance().registerGameObject("gun", std::make_shared<PBRGun>("model/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX"));
    ResourceManager::getInstance().registerRenderPass("pbr_pass", std::make_shared<PBRRenderPass>(ResourceManager::getInstance().getGameObject("gun")));

    App::getInstance().run();

    return 0;
}