#ifndef WORLD_HPP
#define WORLD_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>


#include "../utils/camera.hpp"
#include "../utils/data.hpp"
#include "../utils/music.hpp"
#include "../utils/bezier.hpp"
#include "../gfx/texture.hpp"
#include "../gfx/shader.hpp"
#include "../gfx/text.hpp"
#include "planet.hpp"

enum UpdateMode {NO_UPDATE, UPDATE_DATA};

class World{
    private:
        int nbPlanet;
        std::vector<Planet> planets;
        std::vector<Data> datas;


        Planet outline;
        Mesh fade;

        TextMesh credit;
        Music music;

        Bezier trajectory;

        static float deltaTime;
        static float globalTime;
        static float theta;
        static float time;
        static bool enableRotation;

        UpdateMode updateMode;

        void Init();
        void Start();
        void Render();
        void ProcessInput();
        void UpdateDeltaTime();

        void SetCallback();
        void SetParameters();

        void SetTrajectoryPToP(float time, float timeStart, float timeEnd, int first, int second, float distanceCoeff);
        void MoveRelatedToTrajectory(float time, float timeStart, float timeEnd);
        void ShowPlanet(float time, int indexPlanet, float timeStart, float timeEnd, float distanceCoeff);
        void ShowAllPlanet(float time, float timeStart, float timeEnd);

        // Callback
        static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
        static void MouseCallback(GLFWwindow* , double xpos, double ypos);

    public:
        static GLFWwindow* window;
        static Camera camera;
        static ShaderList shaderList;
        static TextureList textureList;


        

        World();
        ~World();

        void Update();
        

};


#endif