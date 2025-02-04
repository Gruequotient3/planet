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

        Texture(const char* imagePath, GLenum format);
        Texture(unsigned int* pixels, int _width, int _height);
};

#endif
