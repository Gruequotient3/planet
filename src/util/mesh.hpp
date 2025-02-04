#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "../gfx/shader.hpp"

const unsigned int tempindices[] = {1, 3, 0, 1, 2, 3};

const float tempvertices[] = {
    -1, -1, -1,
     1, -1, -1,
     1,  1, -1,
    -1,  1, -1,
};


class Mesh{
    private:
        std::vector<float> normals;

        // vertex information buffer
        int bufvsize = 0;
        float* bufv = NULL;

        // indice buffer
        int bufisize = 0;
        unsigned int* bufi = NULL;

        unsigned int vao, vbo, ebo;

    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        Mesh();
        ~Mesh();

        void GenerateMesh();
        void UpdateNormals();
        void RecalculateNormals();
        void Draw(Shader shader);
};

#endif
