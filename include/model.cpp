#include "model.h"
#include <iostream>


// constructor, expects a filepath to a 3D model.
Model::Model(const std::string &path, bool gamma) : gammaCorrection(gamma) {
    loadModel(path);
}

// draws the model, and thus all its meshes
void Model::draw(Shader &shader) {
    int mesh_sz = meshes.size();
    for (int i = 0; i < mesh_sz; ++i) {
        meshes[i].draw(shader);
    }
}

void Model::loadTexturesNotMarked(const std::string &path, Texture2D::Type type) {
    bool skip = false;
    // check if the texture has already been loaded
    if (textures_loaded.find(path) != textures_loaded.end()) {
        return; // texture already loaded, skip
    } else {
        // if not, load the texture and add it to the map
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(path, type);
        textures_loaded[path] = texture;
        for (auto &mesh : meshes) {
            mesh.addTexture(texture);
        }
    }
}

// loads a model with supported ASSIMP extensions from file and stores the
// resulting meshes in the meshes std::vector.
void Model::loadModel(std::string const &path) {
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString()
                  << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion.
void Model::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects
        // in the scene. the scene contains all the data, node is just to
        // keep stuff organized.
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively
    // process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::PrintMaterialTextureTypes(const aiMaterial *material) {
    for (int type = aiTextureType_NONE + 1; type <= AI_TEXTURE_TYPE_MAX;
         ++type) {
        unsigned int count = material->GetTextureCount((aiTextureType)type);
        if (count > 0) {
            std::cout << "Texture type: " << type << ", count: " << count
                      << std::endl;
        }
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vec; // we declare a placeholder vec since assimp uses its
                       // own std::vector class that doesn't directly
                       // convert to glm's vec3 class so we transfer the
                       // data to this placeholder glm::vec3 first.
        // positions
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.position = vec;
        // normals
        if (mesh->HasNormals()) {
            vec.x = mesh->mNormals[i].x;
            vec.y = mesh->mNormals[i].y;
            vec.z = mesh->mNormals[i].z;
            vertex.normal = vec;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) { // assume all model use only 1 uv
                                       // channel
            // tex coords
            glm::vec2 tex;
            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = tex;

            // ensure has tex_coord bef adding tangent info
            // tangent
            vec.x = mesh->mTangents[i].x;
            vec.y = mesh->mTangents[i].y;
            vec.z = mesh->mTangents[i].z;
            vertex.tangent = vec;
        }

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its
    // triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices
        // std::vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each albedo
    // texture should be named as 'albedo_mapN' where N is a sequential
    // number ranging from 1 to MAX_SAMPLER_NUMBER. Same applies to other
    // texture as the following list summarizes:

    PrintMaterialTextureTypes(material);

    std::vector<std::shared_ptr<Texture2D>> textures;
    // 1. albedo maps
    std::vector<std::shared_ptr<Texture2D>> albedo_maps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, Texture2D::Type::Albedo);
    textures.insert(textures.end(), albedo_maps.begin(), albedo_maps.end());
    // 2. metallic maps
    std::vector<std::shared_ptr<Texture2D>> metallic_maps = loadMaterialTextures(
        material, aiTextureType_METALNESS, Texture2D::Type::Metallic);
    textures.insert(textures.end(), metallic_maps.begin(), metallic_maps.end());
    // 3. normal maps
    std::vector<std::shared_ptr<Texture2D>> normal_maps = loadMaterialTextures(
        material, aiTextureType_NORMALS, Texture2D::Type::Normal);
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    // 4. roughness maps
    std::vector<std::shared_ptr<Texture2D>> roughness_maps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE_ROUGHNESS, Texture2D::Type::Roughness);
    textures.insert(textures.end(), roughness_maps.begin(),
                    roughness_maps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

// checks all material textures of a given assimp_type and loads the
// textures if they're not loaded yet. the required info is returned as a
// Texture struct.
std::vector<std::shared_ptr<Texture2D>>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType assimp_type,
                            Texture2D::Type type) {
    std::vector<std::shared_ptr<Texture2D>> textures;
    unsigned int tex_cnt = mat->GetTextureCount(assimp_type);

    for (unsigned int i = 0; i < tex_cnt; i++) {
        aiString str;
        mat->GetTexture(assimp_type, i, &str);
        // check if texture was loaded before and if so, continue to next
        // iteration: skip loading a new texture
        if (textures_loaded.find(str.C_Str()) != textures_loaded.end()) {
            textures.push_back(textures_loaded[str.C_Str()]);
            continue;
        } else {
            std::string filename = std::string(str.C_Str());
            auto path = directory + "/" + filename;
            auto texture = std::make_shared<Texture2D>(path, type);
            textures.push_back(texture);
            textures_loaded[str.C_Str()] =
                texture; // store it as texture loaded for entire model, to
                         // ensure we won't unnecessary load duplicate
                         // textures.
        }
    }
    return textures;
}