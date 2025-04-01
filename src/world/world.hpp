#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>

#include "planet.hpp"
#include "../util/data.hpp"
#include "sky.hpp"

enum UpdateMode {NO_UPDATE, UPDATE_DATA};

static const std::string dataSettingPaths[]{
    "res/planetConfig/setting/p1.txt", "res/planetConfig/color/p1.txt",
    //"res/planetConfig/setting/p2.txt", "res/planetConfig/color/p2.txt",
    //"res/planetConfig/setting/p3.txt", "res/planetConfig/color/p3.txt",
    //"res/planetConfig/setting/p4.txt", "res/planetConfig/color/p4.txt",
};

static const glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 lightColor = glm::vec3(1.0f, 0.96f, 0.57f);

class World{
    private:
        Sky* sky;

        int nbPlanet;
        Planet* planets;
        Data* datas;

        Shader planetShader;
        Shader skyShader;

        Texture skyTexture;
        void Init();

    public:
        static Camera camera;
        World();
        ~World();
        void Update(float deltaTime, UpdateMode updateMode);
};

#endif
