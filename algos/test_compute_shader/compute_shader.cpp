#include "compute_shader_pass.h"
#include "render_quad_pass.h"
#include "resource_manager.h"
#include "app.h"

int main() {
    App::getInstance().init(800, 800, "Compute-Shader");

    ResourceManager::getInstance().registerRenderPass("compute_shader_pass", std::make_shared<ComputeShaderPass>());
    ResourceManager::getInstance().registerRenderPass("render_quad", std::make_shared<RenderQuadPass>());

    App::getInstance().run();

    return 0;
}