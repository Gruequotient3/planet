#ifndef WORLD_CPP
#define WORLD_CPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "world.hpp"

#include <iostream>
#include <cstdlib>

Camera World::camera = Camera{};
World::World() : planetShader{Shader{"res/shaders/planet.vert", "res/shaders/planet.frag"}},
                 skyShader{Shader{"res/shaders/sky.vert", "res/shaders/sky.frag"}}
{
    glActiveTexture(GL_TEXTURE0);
    skyTexture = Texture("res/textures/sky.jpg", GL_RGB);
    planets = nullptr;
    datas = nullptr;
    Init();
}

World::~World(){
    // Sky
    sky->~Sky();

    // Planets
    for (int i = 0; i < nbPlanet; ++i){
        planets[i].~Planet();
        datas[2 * i].~Data();
        datas[2 * i + 1].~Data();

    }
    free(sky);
    free(planets);
    free(datas);
}

void World::Init(){
    // Sky
    sky = (Sky*) malloc(sizeof(Sky));
    new (sky) Sky{skyShader, skyTexture, 8};
    sky->ConstructMesh();

    // Planets
    nbPlanet = (int)(sizeof(dataSettingPaths) / (sizeof(std::string) * 2));
    planets = (Planet*)malloc(sizeof(Planet) * nbPlanet);
    datas = (Data*) malloc(sizeof(Data) * nbPlanet * 2);
    for (int i = 0; i < nbPlanet; ++i){
        new (planets+i) Planet{planetShader};
        new (datas+ 2 * i) Data();
        new (datas+ 2 * i +1) Data();

        if (i == 0) planets[i].type = SUN;

        datas[2 * i].LoadData(dataSettingPaths[2 * i]);
        datas[2 * i + 1].LoadData(dataSettingPaths[2 * i + 1]);
        planets[i].SetSettingsFromData(datas[2 * i]);
        planets[i].SetColorsFromData(datas[2 * i + 1]);
        planets[i].position = glm::vec3(i * 10.f, 0.0f, 0.0f);
    }


    planetShader.use();
    planetShader.setVec3("lightPos", lightPos);
    planetShader.setVec3("lightColor", lightColor);
}

void World::Update(float deltaTime, UpdateMode updateMode){
    static float theta = 0.0f;
    static float globalTime = 0.0f;
    static float time = 0.0f;
    glm::mat4 model;

    planetShader.setFloat("time", globalTime);
    // Update Data
    if (updateMode == UPDATE_DATA){
        if (time >= 0.5f){
            for (int  i = 0, id = 0; i < nbPlanet; ++i, id += 2){
                if (!datas[id].UpdateData()) planets[i].SetSettingsFromData(datas[id]);
                if (!datas[id+1].UpdateData()) planets[i].SetColorsFromData(datas[id+1]);
            }
            time = 0.0f;
        }
    }

    // Sky
    sky->position = camera.position;
    sky->Draw();

    // Move planets
    for (int i = 0; i < nbPlanet; ++i){
        model = glm::mat4(1.0f);
        planetShader.use();
        if (planets[i].type != SUN){
            model = glm::rotate(model, glm::radians(theta * planets[i].revolutionSpeed),
                                glm::vec3(0.0f , 1.0f, 0.0f));
        }
        model = glm::translate(model, planets[i].position);
        model = glm::rotate(model, glm::radians(theta * planets[i].rotationSpeed),
                            glm::normalize(glm::vec3(0.0f , 1.0f, 0.0f)));

        planetShader.setMat4("model", GL_FALSE, glm::value_ptr(model));
        planets[i].Draw();
    }
    theta += deltaTime * 360.0f;
    if (updateMode != NO_UPDATE) {
        time += deltaTime;
        globalTime += deltaTime;
    }
}

#endif
