#ifndef WORLD_CPP
#define WORLD_CPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "world.hpp"
#include "bezier.hpp"

#include <iostream>
#include <cstdlib>
#include <string>


#define lerp(a, b, t) (a + t * (b - a))
#define inverselerp(a, b, t) ((t - a) / (b - a))

const static ColorSettings sun{
    {1.0f, 1.0f, 0.96f, 0.86f, 0.5f, 0.26f, 0.87f, 0.36f, 0.21f, 0.45f, 0.09f, 0.03f},
    {0.1, 0.2, 0.5, 0.8}
};

const static ColorSettings redSun{
    {1.0f, 1.0f, 0.96f, 0.96f, 0.4f, 0.16f, 0.97f, 0.26f, 0.11f, 0.55f, 0.00f, 0.00f},
    {0.1, 0.2, 0.5, 0.8}
};

const static ColorSettings littleSun{
    {0.95f, 0.95f, 1.0f, 0.95f, 0.95f, 1.0f, 0.95f, 0.95f, 1.0f, 0.95f, 0.95f, 1.0f},
    {0.1, 0.2, 0.5, 0.8}
};

const static ColorSettings deadSun{
    {1.0f, 1.0f, 0.96f, 0.42f, 0.64f, 0.9f, 0.28f, 0.48f, 0.71f, 0.16f, 0.29f, 0.46f},
    {0.1, 0.2, 0.5, 0.8}
};

const static glm::vec3 outcolsun{0.86f, 0.5f, 0.26f};
const static glm::vec3 outcolredsun{1.0f, 0.36f, 0.21};
const static glm::vec3 outcollittlesun{1.0f, 1.0f, 0.96f};
const static glm::vec3 outcoldeadsun{0.06f, 0.33f, 0.67f};

static float sunLightStrength = -1.0f;
static float sunStrength = 0.35f;

Camera World::camera = Camera{};
World::World() : planetShader{Shader{"res/planet.vert", "res/planet.frag"}},
                outlineShader{Shader{"res/planet.vert", "res/outlinePlanet.frag"}}
{
    planets = nullptr;
    datas = nullptr;
    Init();
}

World::~World(){
    // Planets
    for (int i = 0; i < nbPlanet; ++i){
        planets[i].~Planet();
        datas[2 * i].~Data();
        datas[2 * i + 1].~Data();
    }
    outlineSun->~Planet();
    
    if(music){
        if(Mix_PlayingMusic())
          Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    Mix_CloseAudio();
    Mix_Quit();

    free(planets);
    free(outlineSun);
    free(datas);
}

void World::Init(){
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
    }

    srand(0);
    for (int i = 0; i < nbPlanet; ++i){
        planets[i].position = glm::normalize(glm::vec3(rand() / float(RAND_MAX) - 0.5, 0, rand() / float(RAND_MAX) - 0.5)) * (float)i * 10.0f;
    }

    // Sun Outline
    outlineSun = (Planet *) malloc(sizeof(Planet));
    Data outlineSunData;
    new (outlineSun) Planet{outlineShader};

    outlineSunData.LoadData("res//outlineSunSetting.txt");
    outlineSun->SetSettingsFromData(outlineSunData);
    outlineSun->position = glm::vec3(0.0);

    outlineShader.use();
    outlineShader.setVec3("color", glm::vec3(0.86f, 0.5f, 0.26f));
    outlineShader.setVec3("cameraPos", camera.position);
    

    planetShader.use();
    planetShader.setVec3("lightPos", lightPos);
    planetShader.setVec3("lightColor", lightColor);
    planetShader.setFloat("sunLightStrength", sunLightStrength);
    planetShader.setFloat("sunStrength", sunStrength);

    InitMusic("res/modtomit_-_sad_machine.it");

}

void World::Update(float deltaTime, UpdateMode updateMode){
    static float theta = 0.0f;
    static float globalTime = 0.0f;
    static float time = 0.0f;
    static bool enableRotation = false;
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

    // Apply rotation
    for (int i = 0; i < nbPlanet; ++i){
        if (!enableRotation) break;
        if (planets[i].type != SUN){
            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(planets[i].revolutionSpeed * deltaTime * 360),
                                glm::vec3(0.0f , 1.0f, 0.0f));
            planets[i].position = rot * glm::vec4(planets[i].position, 0.0);
        }
    }
    

    if (globalTime < 4.0f){
        float percent = inverselerp(0.0f, 4.0f, globalTime);
        Mix_VolumeMusic(lerp(0.0f, (float)MIX_MAX_VOLUME, percent));
    }
    
    ShowPlanet(globalTime, 1, 0.0f, 6.0f, 1.2f);
    ShowPlanet(globalTime, 2, 6.0f, 12.0f, 1.5);
    ShowPlanet(globalTime, 3, 12.0f, 18.0f, 1.5);
    ShowPlanet(globalTime, 4, 18.0f, 24.0f, 3.0f);
    
    if (globalTime > 24.0f && globalTime < 34.0f){
        enableRotation = true;
        camera.position = normalize(glm::vec3(0.0, 0.3, 1)) * 30.0f;
        camera.front = -glm::normalize(camera.position);
        planets[0].scale += 0.1f * deltaTime;
        float percent = inverselerp(24, 34.0f, globalTime);
        for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
            planets[0].colorSettings.colors[i] = lerp(sun.colors[i], redSun.colors[i], percent);
        }
        outlineShader.use();
        outlineShader.setVec3("color", lerp(outcolsun, outcolredsun, (percent * 10.0f > 1.0f ? 1.0f : percent * 10.0f)));
    }

    if (globalTime >= 34.0f && globalTime < 44.0f){
        planets[0].scale -= 0.2 * deltaTime;
        float percent = inverselerp(34.0f, 44.0f, globalTime);
        for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
            planets[0].colorSettings.colors[i] = lerp(redSun.colors[i], littleSun.colors[i], percent);
        }
        outlineShader.use();
        outlineShader.setVec3("color", lerp(outcolredsun, outcollittlesun, percent));
    }

    if (globalTime > 44.0f && globalTime < 55.55f){
        planets[0].scale += 0.5f * deltaTime;
        float percent = inverselerp(44.0f, 55.55f, globalTime);
        for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
            planets[0].colorSettings.colors[i] = lerp(littleSun.colors[i], deadSun.colors[i], percent);
        }
        planetShader.use();
        planetShader.setFloat("sunLightStrength", sunLightStrength - lerp(0.0f, 10.0, percent));
        outlineShader.use();
        outlineShader.setVec3("color", lerp(outcollittlesun, outcoldeadsun, (percent * 10.0f > 1.0f ? 1.0f : percent * 10.0f)));
        if (globalTime > 51.55f){
            float volumePercent = inverselerp(53.55f, 55.55f, globalTime);
            Mix_VolumeMusic(lerp(MIX_MAX_VOLUME, 0, volumePercent));
        }
    }
    

    if (globalTime < 55.55f){
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw Planet
        for (int i = 0; i < nbPlanet; ++i){
            model = glm::mat4(1.0f);
            planetShader.use();
            model = glm::scale(model, glm::vec3(1.0f) * planets[i].scale);
            model = glm::translate(model, planets[i].position);
            model = glm::rotate(model, glm::radians(theta * planets[i].rotationSpeed),
                                glm::normalize(glm::vec3(0.0f , 1.0f, 0.0f)));
            planetShader.setMat4("model", GL_FALSE, glm::value_ptr(model));
            planets[i].Draw();
        }


        outlineShader.use();
        outlineShader.setVec3("cameraPos", camera.position);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f) * 1.28f * planets[0].scale);
        model = glm::translate(model, outlineSun->position);
        outlineShader.setMat4("model", GL_FALSE, glm::value_ptr(model));
        outlineSun->Draw();
    }
    else{
        glClearColor(1.0, 1.0, 1.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    theta += deltaTime * 360.0f;
    if (updateMode != NO_UPDATE) {
        time += deltaTime;
        globalTime += deltaTime;
    }
}

void World::ShowPlanet(float time, int indexPlanet, float timeStart, float timeEnd, float distanceCoeff){
    if (time >= timeStart && time < timeEnd){
        camera.position = planets[indexPlanet].position - glm::cross(glm::normalize(planets[indexPlanet].position), camera.up) * distanceCoeff;
        camera.front = glm::normalize(planets[indexPlanet].position - camera.position);
    }
}

void World::InitMusic(const char *musicPath){
    int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD, res;
    res = Mix_Init(mixFlags);
    if( (res & mixFlags) != mixFlags ) {
        fprintf(stderr, "Mix_Init: Erreur lors de l'initialisation de la bibliotheque SDL_Mixer\n");
        fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
        //exit(3); commenté car ne réagit correctement sur toutes les architectures
    }
    if(Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0)
        exit(4);
    if(!(music = Mix_LoadMUS(musicPath))) {
        fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
        exit(5);
    }
    Mix_SetPostMix(NULL, NULL);
    if(!Mix_PlayingMusic())
        Mix_PlayMusic(music, 1);
}

#endif
