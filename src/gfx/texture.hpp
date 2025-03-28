#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <glad/glad.h>

#include <iostream>

class Texture{
    public:
        unsigned int id;
        GLenum type;
        int width;
        int height;

        Texture();
        Texture(const char* imagePath, GLenum format);

        void LoadTexture(const char* imagePath, GLenum format);
};

#endif
