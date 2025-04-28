#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>

#include <SDL2/SDL_mixer.h>

#include "planet.hpp"
#include "data.hpp"

#define ECHANTILLONS 1024

enum UpdateMode {NO_UPDATE, UPDATE_DATA};

static const std::string dataSettingPaths[]{
    "res/p1Setting.txt", "res/p1Color.txt",
    "res/p5Setting.txt", "res/p5Color.txt",
    "res/p3Setting.txt", "res/p3Color.txt",
    "res/p2Setting.txt", "res/p2Color.txt",
    "res/p4Setting.txt", "res/p4Color.txt",
};

static const glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, 0.0f);
static const glm::vec3 lightColor = glm::vec3(1.0f, 0.96f, 0.57f);

class World{
    private:
        int nbPlanet;
        Planet* outlineSun;
        Planet* planets;
        Data* datas;

        Shader planetShader;
        Shader outlineShader;

        Mix_Music *music = NULL;

        void Init();
        void ShowPlanet(float time, int indexPlanet, float timeStart, float timeEnd, float distanceCoeff);
        void InitMusic(const char *musicPath);
        
    public:
        static Camera camera;
        World();
        ~World();
        void Update(float deltaTime, UpdateMode updateMode);
};

#endif
