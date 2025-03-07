#ifndef PLANET_CPP
#define PLANET_CPP

#include "planet.hpp"

#include "../util/direction.hpp"

#define NB_PARAMETER 12
#define NB_COLOR 16

Planet::Planet(Shader &shader) : shader{shader}{};

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
    SetMinMax();
}

void Planet::Draw(){
    for (int i = 0; i < 6; ++i){
        terrainFace[i].mesh.Draw(shader);
    }
}

void Planet::SetSettingsFromData(Data &data){
    if (data.data.size() < NB_PARAMETER) {
        std::cout << "SetSettingsFromData: Not enough data for this object" << std::endl;
        return;
    }
    int nbNoiseFilter = (int)((data.data.size()-2) / 7);

    this->resolution = (int)data.data[0];
    this->shapeSettings.radius = data.data[1];
    this->shapeSettings.noiseFilter.clear();

    NoiseSettings newNoiseSettings;
    for (int i = 0; i < nbNoiseFilter; ++i){
        int j = i * 10 + 2;
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

    shader.use();
    shader.setVec3p("colors", nbCurColors, &data.data[0]);
    shader.setFloatp("heights", nbCurColors, &data.data[3 * nbColors]);
    shader.setInt("nbColor", nbCurColors);
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

    shader.use();
    shader.setFloat("minValue", min);
    shader.setFloat("maxValue", max);
}



#endif
