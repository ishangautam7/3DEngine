#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "structures.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

unsigned int TextureFromFile(const char* path, const std::string& directory) {
    std::string filename = directory + '/' + path;
    std::cout << "Attempting to load texture: " << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    
    if (data) {
        GLenum format = (nrComponents == 1) ? GL_RED :
                        (nrComponents == 3) ? GL_RGB : GL_RGBA;
                        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        std::cout << "Texture loaded successfully: " << filename << std::endl;
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        
        // Create a default white texture
        unsigned char whitePixel[4] = {255, 255, 255, 255};
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        std::cout << "Created default white texture." << std::endl;
    }

    return textureID;
}
