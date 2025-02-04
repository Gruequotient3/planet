
#ifndef MESH_CPP
#define MESH_CPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

#include "mesh.hpp"

Mesh::Mesh(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Mesh::~Mesh(){
    if (bufv) delete[] bufv;
    if (bufi) delete[] bufi;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::GenerateMesh(){
    for (int i = 0; i < sizeof(tempvertices) / sizeof(float); ++i){
        vertices.push_back(tempvertices[i]);
        normals.push_back(i);
    }
    for (int i = 0; i < sizeof(tempindices) / sizeof(unsigned int); ++i){
        indices.push_back(tempindices[i]);
    }


    if (bufv) delete[] bufv;
    bufvsize = vertices.size() + normals.size();
    bufv = new float[bufvsize];

    if (bufi) delete[] bufi;
    bufisize = indices.size();
    bufi = new unsigned int[bufisize];



    // Fill vertices buffers
    // 3 vertices coordonate + 3 normals coordonate per vertex
    int type = 0;
    int verti = 0; // vertices index
    int normi = 0; // normals index
    for (int i = 0; i < bufvsize; ){
        switch (type){
            // Vertices
            case 0:
                bufv[i] = vertices[verti];
                bufv[i+1] = vertices[verti+1];
                bufv[i+2] = vertices[verti+2];
                i += 3;
                verti += 3;
                break;
            // Normals
            case 1:
                bufv[i] = normals[normi];
                bufv[i+1] = normals[normi+1];
                bufv[i+2] = normals[normi+2];
                i += 3;
                normi += 3;
                break;
        }
        type = (type + 1) % 2;
    }

    // Fill indices buffers
    for (int i = 0; i < bufisize; ++i){
        bufi[i] = indices[i];
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufvsize * sizeof(float), bufv, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufisize * sizeof(float),
                 bufi, GL_STATIC_DRAW);

    // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


}


// TODO
void Mesh::RecalculateNormals(){
    int nbVertex = vertices.size() / 3;

    for (int i = 0; i < nbVertex; ++i){
        glm::vec3 v1, v2;
        v1 = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i*3 + 2]);
        v2 = glm::vec3(vertices[i+1 * 3], vertices[i+1 * 3 + 1], vertices[i*3+1 + 2]);

    }
}


void Mesh::Draw(Shader shader){
    shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, bufisize, GL_UNSIGNED_INT, 0);
}

#endif
