#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <glad/glad.h>

#include <vector>

enum TextureIndex {
    SKY_TEX,
};

class Texture{
    public:
        unsigned int id;
        GLenum type;
        int width;
        int height;
    
        Texture();
        Texture(const char *imagePath, GLenum format);
        
        void Destroy();
        void LoadTexture(const char *imagePath, GLenum format);
};

class TextureList{
    public: 
        std::vector<Texture> textures;
    
        void Destroy();
        void LoadTexture();

        Texture &operator[](int pos);
};

#endif