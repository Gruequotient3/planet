#ifndef SKY_CPP
#define SKY_CPP

#include "sky.hpp"

#include "world.hpp"
#include "../util/direction.hpp"

Sky::Sky(Shader& shader, Texture& texture, int resolution) : shader{shader}, texture{texture}{
    this->resolution = resolution;
    position = glm::vec3(0.0f);
}

void Sky::ConstructMesh(){
    for (int k = 0; k < 6; ++k){
        glm::vec3 localUp = getDirectionVector((enum Direction)k);
        glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
        glm::vec3 axisB = glm::cross(localUp, axisA);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (int y = 0; y < resolution; ++y){
            for (int x = 0; x < resolution; ++x){
                Vertex vertex;
                int i = x + y * resolution;
                glm::vec2 percent =
                    glm::vec2(x / (float)(resolution - 1), y / (float)(resolution - 1));
                glm::vec3 pointOnUnitCube = localUp
                    + (percent.x - 0.5f) * 2.0f * axisA
                    + (percent.y - 0.5f) * 2.0f * axisB;
                glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
                vertex.position = pointOnUnitSphere;
                vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
                vertex.texCoord = glm::vec2(x / (float) resolution, y / (float) resolution);
                vertices.push_back(vertex);
                if (x != resolution - 1 && y != resolution - 1){
                    indices.push_back(i);
                    indices.push_back(i + resolution + 1);
                    indices.push_back(i + resolution);

                    indices.push_back(i);
                    indices.push_back(i + 1);
                    indices.push_back(i + resolution + 1);
                }
            }
        }
        mesh[k].Clear();
        mesh[k].vertices = vertices;
        mesh[k].indices = indices;
        mesh[k].RecalculateNormal();
        mesh[k].GenerateMesh();
    }
}

void Sky::Draw(){
    glDisable(GL_CULL_FACE);
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(1.0f) * 9000.0f);

    shader.setMat4("model", GL_FALSE, glm::value_ptr(model));
    shader.setMat4("view", GL_FALSE, World::camera.GetViewMatrice());
    shader.setMat4("projection", GL_FALSE, World::camera.GetProjectionMatrice());
    for (int i = 0; i < 6; ++i){
        mesh[i].Draw(shader);
    }
    glEnable(GL_CULL_FACE);
}

#endif
