#include "model.h"
#include <iostream>
#include <stb_image.h>
#include <cstdlib>
#include <cstring>

// Declare a global pointer to the scene so that embedded textures can be accessed in loadMaterialTextures
static const aiScene* gScene = nullptr;

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    // Process flags include triangulation, flipping UVs, calc tangent space, and generating normals
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    // Save pointer to scene so that embedded textures can be accessed later
    gScene = scene;

    // Setup directory based on given path
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Recursively process each child node
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // positions
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        // normals
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Texture> textures;
    // Iterate over all textures of the given type
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Check if texture was already loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }
        if (skip) {
            continue;
        }

        Texture texture;
        // Check for embedded texture: embedded texture file names start with '*'
        if (str.C_Str()[0] == '*') {
            // Extract texture index from string (e.g., "*0" -> index 0)
            int texIndex = std::atoi(str.C_Str() + 1);
            if (gScene && texIndex < static_cast<int>(gScene->mNumTextures)) {
                const aiTexture* aiTex = gScene->mTextures[texIndex];
                unsigned int textureID;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);

                // Check if the texture is compressed or uncompressed
                if (aiTex->mHeight == 0) {
                    // Compressed embedded texture: use stb_image to decode from memory
                    int width, height, nrComponents;
                    unsigned char* data = stbi_load_from_memory(
                        reinterpret_cast<unsigned char*>(aiTex->pcData),
                        aiTex->mWidth,
                        &width, &height, &nrComponents, 0
                    );
                    if (data) {
                        GLenum format = GL_RGB;
                        if (nrComponents == 1)
                            format = GL_RED;
                        else if (nrComponents == 3)
                            format = GL_RGB;
                        else if (nrComponents == 4)
                            format = GL_RGBA;
                        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                        stbi_image_free(data);
                    } else {
                        std::cerr << "Failed to load embedded compressed texture" << std::endl;
                    }
                } else {
                    // Uncompressed texture: assume raw RGBA data
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aiTex->mWidth, aiTex->mHeight, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, aiTex->pcData);
                }
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                texture.id = textureID;
                texture.type = typeName;
                texture.path = std::string(str.C_Str());

                textures.push_back(texture);
                loadedTextures.push_back(texture);
                continue;
            } else {
                std::cerr << "Embedded texture index out of bounds: " << str.C_Str() << std::endl;
            }
        }

        // Fallback: load external texture (usually not needed if textures are embedded)
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = std::string(str.C_Str());
        textures.push_back(texture);
        loadedTextures.push_back(texture);
    }
    return textures;
}

void Model::draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

bool Model::isEmpty() const {
    return meshes.empty();
}
