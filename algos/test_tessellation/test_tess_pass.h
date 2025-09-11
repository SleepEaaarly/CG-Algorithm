#include "render_pass.h"
#include "resource_manager.h"
#include "texture.h"
#include "game_object.h"

class TestTessPass : public IRenderPass {
  public:
    TestTessPass() {
        shader = std::make_shared<Shader>(
            "algos/test_tessellation/height_map.vert",
            "algos/test_tessellation/height_map.frag", nullptr,
            "algos/test_tessellation/height_map.tesc",
            "algos/test_tessellation/height_map.tese");
    }

    virtual void init() override {
        ResourceManager::getInstance().registerTexture2D("height_map", std::make_shared<Texture2D>("algos/test_tessellation/iceland_heightmap.png", Texture2D::Type::None));
        auto tex = ResourceManager::getInstance().getTexture2D("height_map");
        int width = tex->getWidth(), height = tex->getHeight();
        std::vector<Vertex> vertexs;
        int rez = 20;
        for (int i = 0; i <= rez-1; ++i) {
            for (int j = 0; j <= rez-1; ++j) {
                Vertex vert[4];
                vert[0].position = glm::vec3(-width/2.0f+width*i/(float)rez, 0.f, -height/2.0f+height*j/(float)rez);
                vert[0].tex_coords = glm::vec2(i/(float)rez, j/(float)rez);

                vert[1].position = glm::vec3(-width/2.0f+width*(i+1)/(float)rez, 0.f, -height/2.0f+height*j/(float)rez);
                vert[1].tex_coords = glm::vec2((i+1)/(float)rez, j/(float)rez);

                vert[2].position = glm::vec3(-width/2.0f+width*i/(float)rez, 0.f, -height/2.0f+height*(j+1)/(float)rez);
                vert[2].tex_coords = glm::vec2(i/(float)rez, (j+1)/(float)rez);

                vert[3].position = glm::vec3(-width/2.0f+width*(i+1)/(float)rez, 0.f, -height/2.0f+height*(j+1)/(float)rez);
                vert[3].tex_coords = glm::vec2((i+1)/(float)rez, (j+1)/(float)rez);
                vertexs.push_back(vert[0]);
                vertexs.push_back(vert[1]);
                vertexs.push_back(vert[2]);
                vertexs.push_back(vert[3]);
            }
        }

        Model m(vertexs);
        ResourceManager::getInstance().registerGameObject("terrian", std::make_shared<GameObject>(m));

        shader->use();
        shader->setInt("heightMap", 0);

        glPatchParameteri(GL_PATCH_VERTICES, 4);
        // draw wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    virtual void update() override {
        shader->use();
        shader->setMat4("view", ResourceManager::getInstance().getMainCamera().getViewMatrix());
        shader->setMat4("projection", ResourceManager::getInstance().getMainCamera().getProjectionMatrix());
    }

    virtual void render() override {
        auto tex = ResourceManager::getInstance().getTexture2D("height_map");
        auto terrian = ResourceManager::getInstance().getGameObject("terrian");

        glBindTexture(GL_TEXTURE_2D, tex->getId());
        glActiveTexture(GL_TEXTURE0);
        
        terrian->draw(*shader, GL_PATCHES);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
};