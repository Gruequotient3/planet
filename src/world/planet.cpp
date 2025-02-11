#ifndef PLANET_CPP
#define PLANET_CPP

#include "planet.hpp"
#include "../util/direction.hpp"

Planet::Planet(int resolution, float radius){
    this->resolution = resolution;
    this->shapeSettings.radius = radius;
    Initialize();
    GenerateMesh();
}

Planet::Planet(NoiseSettings noiseSettings, int resolution, float radius){
    this->resolution = resolution;
    this->shapeSettings.radius = radius;
    this->shapeSettings.noiseFilter.noise = Noise(noiseSettings);
    Initialize();
    GenerateMesh();
}

void Planet::Initialize(){
    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].Initialize(resolution, getDirectionVector((enum Direction)i),
                                  shapeSettings);
    }
}

void Planet::GenerateMesh(){
    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].ConstructMesh();
    }
}

void Planet::Draw(Shader& shader){
    for (int i = 0; i < 6; ++i){
        terrainFace[i].mesh.Draw(shader);
    }
}


#endif
