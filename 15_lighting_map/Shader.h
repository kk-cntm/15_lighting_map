#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;
    
    Shader(const char* vPath, const char* fPath);
    void use();
    void setValue(const char* name, float value);
    void setValue(const char* name, int value);
    void setValue(const char* name, const glm::mat4& value);
    void setValue(const char* name, const glm::vec3& value);
    
private:
    unsigned int compileShader(const char* src, const GLint type);
    unsigned int compileProgram(unsigned int vShader, unsigned int fShader);
};
