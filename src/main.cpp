#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL_ttf.h>

#include "shader.hpp"
#include "world.hpp"

static int Loop();

static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);

static void UpdateDeltaTime();

static void processInput(GLFWwindow* window);

static float deltaTime = 0.0f;

static int width = 1920;
static int height = 1080;

int main(){
    World::camera = width / (float)height;
    // Initialise GLFW
    if (!glfwInit()){
        std::cout << "Failed to initialise GLFW" << std::endl;
        return -1;
    }
    int loop = Loop();
    glfwTerminate();
    return loop;
}

int Loop(){
    // OpenGL context parameters
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialise OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, "Solar System", NULL, NULL);

    if (!window){
        std::cout << "Failed to initialise OpenGL Context" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Initialise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    World world;

    // Hide Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Callback
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

    // Main loop
    while(!glfwWindowShouldClose(window)){
        UpdateDeltaTime();
        processInput(window);

        world.camera.Update();
        world.Update(deltaTime, UPDATE_DATA);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwDestroyWindow(window);
    return 0;
}


void frameBufferSizeCallBack(GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
    World::camera.ratio = width / (float)height;
}

void UpdateDeltaTime(){
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = ceilf((currentFrame - lastFrame) * 1000) / 1000;
    lastFrame = currentFrame;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
