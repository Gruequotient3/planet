#ifndef SKY_HPP
#define SKY_HPP

#include <glm/glm.hpp>

#include "../util/mesh.hpp"
#include "../util/camera.hpp"
#include "../gfx/texture.hpp"
#include "../gfx/shader.hpp"

class Sky{
    private:
        int resolution;

        Mesh mesh[6];
        Shader& shader;
        Texture& texture;

    public:
        glm::vec3 position;

        Sky(Shader& shader, Texture& texture, int resolution);

        void ConstructMesh();
        void Draw();
};

#endif
