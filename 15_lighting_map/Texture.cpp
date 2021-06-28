#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "Texture.h"

Texture::Texture() {}

Texture::Texture(const char* path, FileType type) {
    GLenum format = type == FileType::png ? GL_RGBA : GL_RGB;
    
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "[ERROR] Failed to load texture with path: " << path << std::endl;
    }
    
    stbi_image_free(data);
}

void Texture::bind(GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
