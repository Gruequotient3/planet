#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/glm.hpp>

#include "../gfx/shader.hpp"


struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh{
    private:
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ebo = 0;

    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh();
        void GenerateMesh();
        void Draw(Shader& shader);
};


#endif
