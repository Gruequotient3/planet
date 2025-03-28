#ifndef TEXTURE_CPP
#define TEXTURE_CPP


#include <stb_image.h>
#include <iostream>

#include "texture.hpp"

Texture::Texture(){
    id = 0;
}

Texture::Texture(const char* imagePath, GLenum format){
    id = 0;
    LoadTexture(imagePath, format);
}

void Texture::LoadTexture(const char* imagePath, GLenum format){
    if (id != 0) glDeleteTextures(1, &id);

    type = GL_TEXTURE_2D;

    // Create Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Texture Parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // load image and set to the texture;
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}


#endif
