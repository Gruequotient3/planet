#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <stb_image.h>
#include <iostream>

#include "texture.hpp"


Texture::Texture() : id{0} { }

void Texture::Destroy(){
    if (id) { glDeleteTextures(1, &id); id = 0; }
}

Texture::Texture(const char *imagePath, GLenum format) : id{0} {
    LoadTexture(imagePath, format);
}

void Texture::LoadTexture(const char *imagePath, GLenum format){
    if (id != 0) glDeleteTextures(1, &id);

    type = GL_TEXTURE_2D;
    
    // Create Texture
    glGenTextures(1, &id);
    glBindTexture(type, id);

    // Texture Parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // load image and set to the texture
    stbi_set_flip_vertically_on_load(false);
    int nrChannels;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(type, 0);
}


void TextureList::Destroy(){
    std::vector<Texture>::iterator it;
    for (it = textures.begin(); it != textures.end(); ++it){
        it->Destroy();
    }
    textures.clear();
}

void TextureList::LoadTexture(){
    Destroy();
    textures.push_back(Texture{"res/texture/sky.jpg", GL_RGBA});
}

Texture &TextureList::operator[](int pos){
    if (pos < 0 || pos >= (int)textures.size()) exit(EXIT_FAILURE);
    return textures[pos];
}


#endif