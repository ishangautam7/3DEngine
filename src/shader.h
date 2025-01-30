#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int ID;
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif