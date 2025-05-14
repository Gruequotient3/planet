#ifndef WORLD_CPP
#define WORLD_CPP

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "world.hpp"
#include "../utils/macro.hpp"


#define WIDTH 1920
#define HEIGHT 1080
#define NB_PARTICLE 50

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

static const glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 lightColor = glm::vec3(1.0f, 0.96f, 0.57f);
static float sunLightStrength = -1.0f;
static float sunStrength = 0.45f;

static const std::string dataSettingPaths[]{
    "res/planetSetting/p1Setting.txt", "res/planetColor/p1Color.txt",
    "res/planetSetting/p5Setting.txt", "res/planetColor/p5Color.txt",
    "res/planetSetting/p3Setting.txt", "res/planetColor/p3Color.txt",
    "res/planetSetting/p2Setting.txt", "res/planetColor/p2Color.txt",
    "res/planetSetting/p4Setting.txt", "res/planetColor/p4Color.txt",
};

GLFWwindow* World::window = nullptr;
Camera World::camera{WIDTH / (float)HEIGHT, WIDTH, HEIGHT};
ShaderList World::shaderList{};
TextureList World::textureList{};

float World::theta = 0.0f;
float World::deltaTime = 0.0f;
float World::globalTime = 0.0f;
float World::time = 0.0f;
bool World::enableRotation = false;

World::World(){
    Init();
    updateMode = UPDATE_DATA;
}

World::~World(){
    for (int i = 0; i < nbPlanet; ++i){
        planets[i].Destroy();
    }    
    outline.Destroy();
    credit.Destroy();

    
    shaderList.Destroy();
    textureList.Destroy();
    
    TextMesh::mesh.Destroy();
    fade.Destroy();

    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}

void World::Init(){
    // Initialise GLFW
    if (!glfwInit()){
        std::cout << "Failed to initialise GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // OpenGL context Parameters
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialise OpenGL context
    window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", glfwGetPrimaryMonitor(), NULL);
    if (!window){
        std::cout << "Failed to initialise OpenGL Context" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // Initialise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialise GLAD" << std::endl;
        glfwTerminate();
        glfwDestroyWindow(window);
        exit(EXIT_FAILURE);
    }
    
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    
    SetParameters();
    SetCallback();


    shaderList.LoadShader();
    textureList.LoadTexture();

    TextMesh::mesh.GenerateMesh();
    fade = Mesh::GetQuadMesh();
    fade.GenerateMesh();
    Start();
}

void World::Start(){
    nbPlanet = (int)(sizeof(dataSettingPaths) / (sizeof(std::string) * 2));
    for (int i = 0; i < nbPlanet; ++i){

        Planet planet;
        Data setting;
        Data color;
        
        if (i == 0) planet.type = SUN;
    
        setting.LoadData(dataSettingPaths[2 * i]);
        color.LoadData(dataSettingPaths[2 * i + 1]);
        
        planet.SetSettingsFromData(setting);
        planet.SetColorsFromData(color);
        
        planets.push_back(planet);
        datas.push_back(setting);
        datas.push_back(color);
        
        planet.Destroy();
    }

    srand(0);
    for (int i = 0; i < nbPlanet; ++i){
        planets[i].position = glm::normalize(glm::vec3(0.5f, 0.0, 0.5f)) * (float)i * 10.0f;
        
    }

    Data outlineData;
    outlineData.LoadData("res/planetSetting/outlineSetting.txt");
    outline.SetSettingsFromData(outlineData);

    shaderList[OUTLINE_SHAD].Use();
    shaderList[OUTLINE_SHAD].SetVec3("color", glm::vec3(0.86f, 0.5f, 0.26f));
    shaderList[OUTLINE_SHAD].SetVec3("cameraPos", camera.position);

    shaderList[PLANET_SHAD].Use();
    shaderList[PLANET_SHAD].SetVec3("lightPos", lightPos);
    shaderList[PLANET_SHAD].SetVec3("lightColor", lightColor);
    shaderList[PLANET_SHAD].SetFloat("sunLightStrength", sunLightStrength);
    shaderList[PLANET_SHAD].SetFloat("sunStrength", sunStrength);

    shaderList[FADE_SHAD].SetVec3("color", glm::vec3(0.0, 0.0, 0.0));


    credit.Init("Demo created with OpenGL, Glad, GLFW, GLM, SDL\n" 
        "Made by : LEVASTRE Clément\n\n"
        "Music : modtomit - sad_machine (modarchive.org)\n"
        "Star texture : startline (freepik.com)");
    music.Init("res/modtomit_-_sad_machine.it");
}

void World::Update(){
    while(!glfwWindowShouldClose(window)){             
        UpdateDeltaTime();
        camera.Update();

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

        if (globalTime < 6.0f){
            float percent = inverseLerp(0.0f, 6.0f, globalTime);
            music.SetVolume(percent, false);
        }

        ShowAllPlanet(globalTime, 0.0f, 24.0f);

        if (globalTime > 24.0f && globalTime < 34.0f){
            enableRotation = true;
            camera.position = normalize(glm::vec3(0.0, 0.3, 1)) * 30.0f;
            camera.front = -glm::normalize(camera.position);
            planets[0].scale += 0.1f * deltaTime;
            float percent = inverseLerp(24, 34.0f, globalTime);
            for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
                planets[0].colorSettings.colors[i] = lerp(sun.colors[i], redSun.colors[i], percent);
            }
            shaderList[OUTLINE_SHAD].Use();
            shaderList[OUTLINE_SHAD].SetVec3("color", lerp(outcolsun, outcolredsun, (percent * 10.0f > 1.0f ? 1.0f : percent * 10.0f)));
        }
    
        if (globalTime >= 34.0f && globalTime < 44.0f){
            planets[0].scale -= 0.2 * deltaTime;
            float percent = inverseLerp(34.0f, 44.0f, globalTime);
            for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
                planets[0].colorSettings.colors[i] = lerp(redSun.colors[i], littleSun.colors[i], percent);
            }
            shaderList[OUTLINE_SHAD].Use();
            shaderList[OUTLINE_SHAD].SetVec3("color", lerp(outcolredsun, outcollittlesun, percent));
        }
    
        if (globalTime > 44.0f && globalTime < 55.55f){
            planets[0].scale += 0.5f * deltaTime;
            float percent = inverseLerp(44.0f, 55.55f, globalTime);
            for (int i = 0; i < (int)planets[0].colorSettings.colors.size(); ++i){
                planets[0].colorSettings.colors[i] = lerp(littleSun.colors[i], deadSun.colors[i], percent);
            }
            shaderList[PLANET_SHAD].Use();
            shaderList[PLANET_SHAD].SetFloat("sunLightStrength", sunLightStrength - lerp(0.0f, 10.0, percent));
            shaderList[OUTLINE_SHAD].Use();
            shaderList[OUTLINE_SHAD].SetVec3("color", lerp(outcollittlesun, outcoldeadsun, (percent * 10.0f > 1.0f ? 1.0f : percent * 10.0f)));
            if (globalTime > 51.55f){
                float volumePercent = inverseLerp(51.55f, 55.55f, globalTime);
                music.SetVolume(volumePercent, true);
            }
        }
        
        Render();
        ProcessInput();

        theta += deltaTime * 360.0f;
        time += deltaTime;
        globalTime += deltaTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void World::Render(){
    glm::mat4 model;
    shaderList[PLANET_SHAD].SetFloat("time", globalTime);
    
    if (globalTime < 55.55f){
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw Planet
        for (int i = 0; i < nbPlanet; ++i){
            model = glm::mat4(1.0f);
            shaderList[PLANET_SHAD].Use();
            model = glm::scale(model, glm::vec3(1.0f) * planets[i].scale);
            model = glm::translate(model, planets[i].position);
            model = glm::rotate(model, glm::radians(theta * planets[i].rotationSpeed),
                                glm::normalize(glm::vec3(0.0f , 1.0f, 0.0f)));
            shaderList[PLANET_SHAD].SetMat4("model", GL_FALSE, glm::value_ptr(model));
            planets[i].Draw(shaderList[PLANET_SHAD]);
        }

        // Draw Sun Outline
        shaderList[OUTLINE_SHAD].Use();
        shaderList[OUTLINE_SHAD].SetVec3("cameraPos", camera.position);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f) * 1.28f * planets[0].scale * 2.0f);
        model = glm::translate(model, outline.position);
        shaderList[OUTLINE_SHAD].SetMat4("model", GL_FALSE, glm::value_ptr(model));
        outline.Draw(shaderList[OUTLINE_SHAD]);

        // Draw Sky
        glDisable(GL_CULL_FACE);
        shaderList[SKY_SHAD].Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureList[SKY_TEX].id);
        model = glm::mat4(1.0f);
        model = glm::translate(model, camera.position);
        model = glm::scale(model ,glm::vec3(1.0f) * 500.0f);
        shaderList[SKY_SHAD].SetMat4("model", GL_FALSE, glm::value_ptr(model));
        outline.Draw(shaderList[SKY_SHAD]);
        glEnable(GL_CULL_FACE);


        shaderList[FADE_SHAD].Use();
        if (globalTime < 1.0f){
            shaderList[FADE_SHAD].SetFloat("percent", inverseLerp(1.0f, 0.0f, globalTime));
        }

        else if (globalTime > 23.0f && globalTime < 24.0f){
            shaderList[FADE_SHAD].SetFloat("percent", inverseLerp(23.0f, 24.0f, globalTime));
        }

        else if (globalTime > 24.0f && globalTime < 25.0f){
            shaderList[FADE_SHAD].SetFloat("percent", inverseLerp(25.0f, 24.0f, globalTime));
        }

        else if (globalTime > 50.0f && globalTime < 54.55f){
            shaderList[FADE_SHAD].SetFloat("percent", inverseLerp(50.0f, 54.55f, globalTime));
            shaderList[FADE_SHAD].SetVec3("color", glm::vec3(1.0, 1.0, 1.0));

        }

        // Draw Fade
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0f, 1.0, 1.0));
        model = glm::translate(model, camera.position + camera.front);
        shaderList[FADE_SHAD].SetMat4("model", GL_FALSE, glm::value_ptr(model));
        shaderList[FADE_SHAD].SetMat4("projection", GL_FALSE, camera.GetPerspectiveMatrice());
        shaderList[FADE_SHAD].SetMat4("view", GL_FALSE, camera.GetViewMatrice());
        fade.Draw(shaderList[FADE_SHAD]);   
    }

    else{
        glClearColor(1.0, 1.0, 1.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.position = glm::vec3(0.0, 0.0, 4.0f);
        camera.front = glm::vec3(0.0, 0.0, -1.0f);
        
        shaderList[TEXT_SHAD].Use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, credit.position);
        model = glm::scale(model, glm::vec3(5.0f, 2.5f, 1.0f));
        shaderList[TEXT_SHAD].SetMat4("model", GL_FALSE, glm::value_ptr(model));
        shaderList[TEXT_SHAD].SetVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
        credit.Render(shaderList[TEXT_SHAD]);
    }
}

void World::ProcessInput(){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


void World::UpdateDeltaTime(){
    static float lastFrame = 0.0f;
    if (lastFrame == 0.0f) lastFrame = glfwGetTime();
    float currentFrame = glfwGetTime();
    deltaTime = ceilf((currentFrame - lastFrame) * 1000) / 1000;
    lastFrame = currentFrame;
}
void World::SetCallback(){
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
    //glfwSetCursorPosCallback(window, MouseCallback);
}

void World::SetParameters(){
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hide Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void World::FrameBufferSizeCallBack(GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
    World::camera.ratio = width / (float)height;
}
void World::MouseCallback(GLFWwindow* , double xpos, double ypos){
    static float lastX = xpos;
    static float lastY = ypos;


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 4.0f;

    xoffset *= sensitivity * deltaTime;
    yoffset *= sensitivity * deltaTime;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f){
        camera.pitch = 89.0f;
    }
    if (camera.pitch < -89.0f){
        camera.pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

    camera.front = glm::normalize(direction);
}

void World::SetTrajectoryPToP(float time, float timeStart, float timeEnd, int first, int second, float distanceCoeff){
    static int lastFirst = -1;
    if (time < timeStart ||time > timeEnd ) return;
    
    if (lastFirst != first){
        lastFirst = first;
        trajectory.points.clear();
        
        trajectory.points.push_back(camera.position); 
        glm::vec3 ptoP = planets[second].position - planets[first].position;
        
        trajectory.points.push_back( 
            (camera.position + ptoP * 0.25f) - 
            glm::cross(glm::normalize(ptoP), camera.up) * distanceCoeff * 2.5f);

        trajectory.points.push_back( 
            (camera.position + ptoP * 0.5f) - 
            glm::cross(glm::normalize(ptoP), camera.up) * distanceCoeff * 5.0f);

        trajectory.points.push_back( 
            (camera.position + ptoP * 0.75f) - 
            glm::cross(glm::normalize(ptoP), camera.up) * distanceCoeff * 2.5f);
        
        trajectory.points.push_back(planets[second].position - 
            glm::cross(glm::normalize(planets[second].position), camera.up) * distanceCoeff);
    }
}

void World::MoveRelatedToTrajectory(float time, float timeStart, float timeEnd){
    if (time >= timeStart && time < timeEnd){
        float percent = inverseLerp(timeStart, timeEnd, time); 
        camera.position = trajectory.Calculate(percent);
    }
}

void World::ShowPlanet(float time, int indexPlanet, float timeStart, float timeEnd, float distanceCoeff){
    if (time >= timeStart && time < timeEnd){
        camera.position = planets[indexPlanet].position - glm::cross(glm::normalize(planets[indexPlanet].position), camera.up) * distanceCoeff;
        camera.front = glm::normalize(planets[indexPlanet].position - camera.position);
    }
}

void World::ShowAllPlanet(float time, float timeStart, float timeEnd){
    if (time < timeStart ||time > timeEnd ) return;
    
    // Show EachPlanet
    ShowPlanet(time, 1, 0.0f, 5.0f, 1.2f);
    SetTrajectoryPToP(time, 5.0f, 6.0f, 1, 2, 2.0f);
    MoveRelatedToTrajectory(time, 5.0f, 6.0f);

    ShowPlanet(time, 2, 6.0f, 11.0f, 2.0f);
    SetTrajectoryPToP(time, 11.0f, 12.0f, 2, 3, 1.5f);
    MoveRelatedToTrajectory(time, 11.0f, 12.0f);


    ShowPlanet(time, 3, 12.0f, 17.0f, 1.5f);
    SetTrajectoryPToP(time, 17.0f, 18.0f, 3, 4, 4.0f);
    MoveRelatedToTrajectory(time, 17.0f, 18.0f);

    ShowPlanet(time, 4, 18.0f, 24.0f, 4.0f);
}

#endif