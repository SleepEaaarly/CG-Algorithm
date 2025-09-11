#include "app.h"
#include "resource_manager.h"
#include "test_tess_pass.h"

int main() {
    App::getInstance().init(800, 800, "Test-Tessellation");

    ResourceManager::getInstance().setupMainCamera(glm::vec3(67.0f, 627.5f, 169.9f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -128.1f, -42.4f, 0.1f, 100000.0f);
    ResourceManager::getInstance().registerRenderPass("tess-pass", std::make_shared<TestTessPass>());

    App::getInstance().run();

    return 0;
}