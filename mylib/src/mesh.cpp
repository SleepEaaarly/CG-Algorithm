#include "mesh.h"
#include "texture.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
           std::vector<std::shared_ptr<Texture2D>> &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::addTexture(std::shared_ptr<Texture2D> tex) { textures.push_back(tex); }

void Mesh::draw(Shader& shader, GLenum mode) {
    unsigned int albedo_num = 1;
    unsigned int metallic_num = 1;
    unsigned int normal_num = 1;
    unsigned int roughness_num = 1;
    unsigned int ao_num = 1;
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(
            GL_TEXTURE0 +
            i); // 绑定纹理对象前激活对应的纹理单元 g_obj[unit][object]
        // 获取纹理编号
        std::string number;
        std::string name;
        auto tex = textures[i];
        switch (tex->getType()) {
        case Texture2D::Type::Albedo:
            name = "albedo_map";
            number = std::to_string(albedo_num++);
            break;
        case Texture2D::Type::Metallic:
            name = "metallic_map";
            number = std::to_string(metallic_num++);
            break;
        case Texture2D::Type::Normal:
            name = "normal_map";
            number = std::to_string(normal_num++);
            break;
        case Texture2D::Type::Roughness:
            name = "roughness_map";
            number = std::to_string(roughness_num++);
            break;
        case Texture2D::Type::AO:
            name = "ao_map";
            number = std::to_string(ao_num++);
            break;
        default:
            break;
        }

        assert(!name.empty() && "Texture type is None or not set");

        shader.setInt((name + number).c_str(), i);
        // 将shader中的 sampler 绑定对应的纹理单元(texture_unit)
        glBindTexture(GL_TEXTURE_2D,
                      textures[i]->getId()); // 绑定纹理对象 GL_TEXTURE_2D
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once
    // configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 这是因为结构体内存布局是连续的
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    // tex coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, tex_coords));
    // tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, tangent));

    glBindVertexArray(0);
}