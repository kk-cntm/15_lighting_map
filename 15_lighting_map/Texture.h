#pragma once

#include <glad/glad.h>

class Texture {
public:
    enum FileType { png, jpg };
    
    unsigned int ID;
    
    Texture(const char* path, FileType type);
    Texture();
    void bind(GLenum unit);
};
