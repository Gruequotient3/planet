#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


#include "shader.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh{
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;

        Mesh();
        ~Mesh();

        
        void GenerateMesh();
        void DestroyMesh();
        void Clear();
        void RecalculateNormal();


        void Draw(Shader &shader);

        static Mesh GetQuadMesh();
};


#endif
