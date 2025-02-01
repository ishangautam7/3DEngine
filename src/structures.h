#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <glm/glm.hpp>
#include <string>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned int TextureFromFile(const char* path, const std::string& directory);

#endif
