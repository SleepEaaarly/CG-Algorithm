#include "resource_manager.h"
#include "shadow_pass.h"
#include "app.h"
#include "visualize_shadowmap_pass.h"
#include "main_render_pass.h"

int main() {
    App::getInstance().init(800, 800, "Shadow");
    ResourceManager::getInstance().registerPreRenderPass("shadow_pass", std::make_shared<ShadowPass>());
    // ResourceManager::getInstance().registerRenderPass("visualize_shadowmap_pass", std::make_shared<VisualizePass>());
    ResourceManager::getInstance().registerRenderPass("main_pass", std::make_shared<RenderPass>());

    App::getInstance().run();

    return 0;
}
