
#ifndef MESH_CPP
#define MESH_CPP

#include <glad/glad.h>

#include <iostream>

#include "mesh.hpp"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices = indices;

    GenerateMesh();
}


Mesh::~Mesh(){
    if (vao) {glDeleteVertexArrays(1, &vao); vao = 0;}
    if (vbo) {glDeleteBuffers(1, &vbo); vbo = 0;}
    if (ebo) {glDeleteBuffers(1, &ebo); ebo = 0;}
}


void Mesh::GenerateMesh(){
    if (!vao) glGenVertexArrays(1, &vao);
    if (!vbo) glGenBuffers(1, &vbo);
    if (!ebo) glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));

    // TexCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Mesh::Clear(){
    vertices.clear();
    indices.clear();
    if (vao) {glDeleteVertexArrays(1, &vao); vao = 0;}
    if (vbo) {glDeleteBuffers(1, &vbo); vbo = 0;}
    if (ebo) {glDeleteBuffers(1, &ebo); ebo = 0;}
}


void Mesh::RecalculateNormal(){
    for (int i = 0; i < (int)vertices.size(); ++i)
        vertices[i].normal = glm::vec3(0.0f);
    for (int i = 0; i < (int)indices.size(); i += 3){
        int vertA = indices[i];
        int vertB = indices[i+1];
        int vertC = indices[i+2];


        glm::vec3 edgeAB = vertices[vertB].position - vertices[vertA].position;
        glm::vec3 edgeAC = vertices[vertC].position - vertices[vertA].position;

        glm::vec3 areaWeightedNormal = glm::cross(edgeAB, edgeAC);

        vertices[vertA].normal += areaWeightedNormal;
        vertices[vertB].normal += areaWeightedNormal;
        vertices[vertC].normal += areaWeightedNormal;

    }
    for (int i = 0; i < (int)vertices.size(); ++i)
        vertices[i].normal = glm::normalize(vertices[i].normal);
}


void Mesh::Draw(Shader& shader){
    shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

#endif
