#ifndef PLANET_CPP
#define PLANET_CPP

#include <iostream>
#include <thread>

#include "planet.hpp"

#include "world.hpp"
#include "direction.hpp"


#define NB_PARAMETER 15
#define NB_COLOR 16

Planet::Planet(Shader &shader) : position{glm::vec3(0.0f)}, scale{1.0f}, type{PLANET}, shader{shader}{}

void Planet::Initialize(){
    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].Initialize(resolution, getDirectionVector((enum Direction)i),
                                  shapeSettings);
    }
}

void Planet::GenerateMesh(){
    std::thread thread[TERRAINFACES];
    for (int i = 0; i < TERRAINFACES; ++i){
        thread[i] = std::thread(&TerrainFace::ConstructMesh, std::ref(terrainFace[i]));
    }
    for (int i = 0; i < TERRAINFACES; ++i){
        if (thread[i].joinable()){
            thread[i].join();
        }
        terrainFace[i].mesh.GenerateMesh();
    }
    SetMinMax();
}

void Planet::Draw(){
    shader.use();
    shader.setMat4("view", GL_FALSE, World::camera.GetViewMatrice());
    shader.setMat4("projection", GL_FALSE, World::camera.GetProjectionMatrice());

    if (type == SUN) shader.setInt("isSun", 1);
    else shader.setInt("isSun", 0);

    shader.setFloat("minValue", min);
    shader.setFloat("maxValue", max);

    shader.setVec3p("colors", (int)colorSettings.colors.size() / 3, &colorSettings.colors[0]);
    shader.setFloatp("heights", (int)colorSettings.heights.size(), &colorSettings.heights[0]);
    shader.setInt("nbColor", colorSettings.heights.size());

    for (int i = 0; i < TERRAINFACES; ++i){
        terrainFace[i].mesh.Draw(shader);
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
    for (int i = 0; i < nbNoiseFilter; ++i){
        int j = i * 10 + 5;
        this->shapeSettings.noiseFilter.push_back(NoiseFilter{});
        this->shapeSettings.noiseFilter[i].enable = (bool)data.data[j];
        this->shapeSettings.noiseFilter[i].useFirstLayerAsMask = (bool)data.data[j+1];
        this->shapeSettings.noiseFilter[i].type = (NoiseFilterType)data.data[j+2];
        newNoiseSettings.strength = data.data[j + 3];
        newNoiseSettings.octave = (int)data.data[j + 4];
        newNoiseSettings.amplitude = data.data[j + 5];
        newNoiseSettings.frequency = data.data[j + 6];
        newNoiseSettings.persistence = data.data[j + 7];
        newNoiseSettings.lucanarity = data.data[j + 8];
        newNoiseSettings.minValue = data.data[j + 9];
        this->shapeSettings.noiseFilter[i].noise = Noise(newNoiseSettings);
    }
    Initialize();
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

void Planet::SetShader(Shader &shader){
    this->shader = shader;
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
