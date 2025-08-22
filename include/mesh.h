#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <memory>
#include <vector>

class Texture2D;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
};

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture2D>> textures;

    unsigned int VAO;

    // constructor
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
         std::vector<std::shared_ptr<Texture2D>> &textures);

    void addTexture(std::shared_ptr<Texture2D> tex);

    void draw(Shader& shader);

  private:
    unsigned int VBO, EBO;

    void setupMesh();
};

#endif