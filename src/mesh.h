#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "shader.h"
#include "structures.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    Mesh(const std::vector<Vertex>& vertices, 
         const std::vector<unsigned int>& indices,
         const std::vector<Texture>& textures);
    void draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
