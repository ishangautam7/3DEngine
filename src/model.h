#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "shader.h"
#include <vector>
#include <string>
#include "structures.h"

class Model {
public:
    Model(const std::string& path);
    void draw(Shader& shader);
    bool isEmpty() const;

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loadedTextures;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, 
        aiTextureType type, const std::string& typeName);
};

#endif
