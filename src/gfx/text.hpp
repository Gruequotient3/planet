#ifndef TEXT_HPP
#define TEXT_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <SDL_ttf.h>

#include <string>

#include "../utils/mesh.hpp"

class TextMesh{
    private:
    public:
        static Mesh mesh;

        unsigned int textureId;

        glm::vec3 position;

        TextMesh();
        void Destroy();

        void Init(std::string text);
        void Render(Shader &shader);
};

#endif