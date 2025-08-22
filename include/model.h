#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include <string>
#include <unordered_map>

class Model {
  public:
    std::unordered_map<std::string, std::shared_ptr<Texture2D>>
        textures_loaded; // stores all the textures loaded so far, optimization
                         // to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection; // no use so far

    // constructor, expects a filepath to a 3D model.
    Model(const std::string &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void draw(Shader &shader);

    void loadTexturesNotMarked(const std::string &path, Texture2D::Type type);

  private:
    // loads a model with supported ASSIMP extensions from file and stores the
    // resulting meshes in the meshes std::vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion.
    void processNode(aiNode *node, const aiScene *scene);

    static void PrintMaterialTextureTypes(const aiMaterial *material);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given assimp_type and loads the
    // textures if they're not loaded yet. the required info is returned as a
    // Texture struct.
    std::vector<std::shared_ptr<Texture2D>>
    loadMaterialTextures(aiMaterial *mat, aiTextureType assimp_type,
                         Texture2D::Type type);
};

#endif