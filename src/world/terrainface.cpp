#ifndef TERRAIN_FACE_CPP
#define TERRAIN_FACE_CPP

#include <vector>
#include <cmath>
#include <iostream>

#include "terrainface.hpp"


TerrainFace::TerrainFace(ShapeSettings &shapeSettings, int resolution, glm::vec3 localUp){
    this->shapeSettings = shapeSettings;
    Initialize(resolution, localUp);
}

void TerrainFace::Initialize(int resolution, glm::vec3 localUp){
    this->resolution = resolution;
    this->localUp = localUp / 4.0f;
    axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
    axisB = glm::cross(localUp, axisA);
}

void TerrainFace::Initialize(int resolution, glm::vec3 localUp, ShapeSettings &shapeSettings){
    this->resolution = resolution;
    this->localUp = localUp / 4.0f;
    this->shapeSettings = shapeSettings;
    axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
    axisB = glm::cross(localUp, axisA);
}


void TerrainFace::ConstructMesh(){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int y = 0; y < resolution; ++y){
        for (int x = 0; x < resolution; ++x){
            Vertex vertex;
            int i = x + y * resolution;
            glm::vec2 percent = glm::vec2(x - (resolution - 1) / 2.0f, y - (resolution-1) / 2.0f)
                / (float)(resolution - 1) / 2.0f;
            glm::vec3 pointOnUnitCube = localUp
                + (percent.x * 0.5f) * 2.0f * axisA
                + (percent.y * 0.5f) * 2.0f * axisB;
            glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
            vertex.position = shapeSettings.CalculatePointOnPlanet(pointOnUnitSphere);
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
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
    mesh.Clear();
    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.RecalculateNormal();
    mesh.GenerateMesh();
}

#endif
