#ifndef PLANET_CPP
#define PLANET_CPP

#include <iostream>
#include <thread>

#include "planet.hpp"
#include "world.hpp"
#include "../utils/direction.hpp"

#define NB_PARAMETER 15
#define NB_COLOR 16

Planet::Planet() : position{glm::vec3(0.0f)}, scale{1.0f}, type{PLANET} { }
Planet::Planet(const Planet &pt){
    resolution = pt.resolution;
    position = pt.position;
    scale = pt.scale;
    type  = pt.type;
    rotationSpeed = pt.rotationSpeed;
    revolutionSpeed = pt.revolutionSpeed;
    
    min = pt.min;
    max = pt.max;
    
    colorSettings = pt.colorSettings;
    shapeSettings = pt.shapeSettings;
    
    Init();
    GenerateMesh();
}

Planet& Planet::operator=(const Planet &pt){
    resolution = pt.resolution;
    position = pt.position;
    scale = pt.scale;
    type  = pt.type;
    rotationSpeed = pt.rotationSpeed;
    revolutionSpeed = pt.revolutionSpeed;
    
    min = pt.min;
    max = pt.max;
    
    colorSettings = pt.colorSettings;
    shapeSettings = pt.shapeSettings;
    
    Init();
    GenerateMesh();
    return *this;
}


void Planet::Init(){
    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].Init(resolution, getDirectionVector((enum Direction)i),
                                  shapeSettings);
    }
}

void Planet::Destroy(){
    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].Destroy();
    }
}

void Planet::GenerateMesh(){
    std::thread thread[TERRAINFACES];
    for (int i = 0; i < TERRAINFACES; ++i){
        thread[i] = std::thread(&TerrainFace::GenerateMesh, std::ref(terrainFace[i]));
    }
    for (int i = 0; i < TERRAINFACES; ++i){
        if (thread[i].joinable()){
            thread[i].join();
        }
        terrainFace[i].mesh.GenerateMesh();
    }
    SetMinMax();
}

void Planet::Draw(Shader &shader){
    shader.Use();
    shader.SetMat4("view", GL_FALSE, World::camera.GetViewMatrice());
    shader.SetMat4("projection", GL_FALSE, World::camera.GetPerspectiveMatrice());

    if (type == SUN) shader.SetInt("isSun", 1);
    else shader.SetInt("isSun", 0);

    shader.SetFloat("minValue", min);
    shader.SetFloat("maxValue", max);

    shader.SetVec3p("colors", (int)colorSettings.colors.size() / 3, &colorSettings.colors[0]);
    shader.SetFloatp("heights", (int)colorSettings.heights.size(), &colorSettings.heights[0]);
    shader.SetInt("nbColor", colorSettings.heights.size());

    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].Draw(shader);
    }
}

void Planet::SetSettingsFromData(Data &data){
    if (data.data.size() < NB_PARAMETER) {
        std::cout << "SetSettingsFromData: Not enough data for this object" << std::endl;
        return;
    }
    int nbNoiseFilter = (int)((data.data.size()-5) / 10);

    this->resolution = (int)data.data[0];
    this->shapeSettings.SetSeed((int)data.data[1]);
    this->shapeSettings.radius = data.data[2];
    this->rotationSpeed = data.data[3];
    this->revolutionSpeed = data.data[4];

    this->shapeSettings.noiseFilter.clear();

    NoiseSettings newNoiseSettings;
    NoiseFilter newNoiseFilter;
    for (int i = 0; i < nbNoiseFilter; ++i){
        int j = i * 10 + 5;
        newNoiseFilter.enable = (bool)data.data[j];
        newNoiseFilter.useFirstLayerAsMask = (bool)data.data[j+1];
        newNoiseFilter.type = (NoiseFilterType)data.data[j+2];
        this->shapeSettings.noiseFilter.push_back(newNoiseFilter);

        newNoiseSettings.strength = data.data[j + 3];
        newNoiseSettings.octave = (int)data.data[j + 4];
        newNoiseSettings.amplitude = data.data[j + 5];
        newNoiseSettings.frequency = data.data[j + 6];
        newNoiseSettings.persistence = data.data[j + 7];
        newNoiseSettings.lucanarity = data.data[j + 8];
        newNoiseSettings.minValue = data.data[j + 9];
        this->shapeSettings.noiseFilter[i].noise = Noise(newNoiseSettings);
    }
    Init();
    GenerateMesh();
}

void Planet::SetColorsFromData(Data &data){
    int nbColors = data.data.size() / 4;
    int nbCurColors = (nbColors <= 16) ? nbColors : 16;

    colorSettings.colors.clear();
    colorSettings.heights.clear();
    for (int i = 0, ih = 3 * nbColors; i < nbCurColors; ++i, ++ih){
        colorSettings.colors.push_back(data.data[3 * i]);
        colorSettings.colors.push_back(data.data[3 * i + 1]);
        colorSettings.colors.push_back(data.data[3 * i + 2]);
        colorSettings.heights.push_back(data.data[ih]);
    }
}

void Planet::SetMinMax(){
    float min = 10000000;
    float max = -1;
    for (int i = 0; i < TERRAINFACES; ++i){
        for (int j = 0; j < (int)terrainFace[i].mesh.vertices.size(); ++j){
            float length = glm::length(terrainFace[i].mesh.vertices[j].position);
            if (length < min) min = length;
            if (length > max) max = length;
        }
    }
    this->min = min;
    this->max = max;
}



#endif
