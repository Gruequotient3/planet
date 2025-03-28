#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "gfx/shader.hpp"
#include "gfx/texture.hpp"
#include "util/camera.hpp"
#include "world/world.hpp"


static int Loop();

static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

static void UpdateDeltaTime();

static void processInput(GLFWwindow* window);

static float deltaTime = 0.0f;
static float cameraSpeed = 4.0f;

static bool firstMouse = true;
static float lastX;
static float lastY;

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
    window = glfwCreateWindow(width, height, "Hello World !", NULL, NULL);

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

    World world;

    // Hide Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Main loop
    while(!glfwWindowShouldClose(window)){

        UpdateDeltaTime();
        processInput(window);
        glClearColor(0.0, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void mouseCallback(GLFWwindow* , double xpos, double ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;


    const float sensitivity = 4.0f;

    xoffset *= sensitivity * deltaTime;
    yoffset *= sensitivity * deltaTime;

    World::camera.yaw += xoffset;
    World::camera.pitch += yoffset;

    if (World::camera.pitch > 89.0f){
        World::camera.pitch = 89.0f;
    }
    if (World::camera.pitch < -89.0f){
        World::camera.pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(World::camera.yaw)) * cos(glm::radians(World::camera.pitch));
    direction.y = sin(glm::radians(World::camera.pitch));
    direction.z = sin(glm::radians(World::camera.yaw)) * cos(glm::radians(World::camera.pitch));

    World::camera.front = glm::normalize(direction);

}

void scrollCallback(GLFWwindow*, double, double yoffset){

    World::camera.fov -= (float)yoffset;
    if (World::camera.fov < 1.0f){
        World::camera.fov = 1.0f;
    }
    if (World::camera.fov > 90.0f){
        World::camera.fov = 90.0f;
    }
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

    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        World::camera.position +=  World::camera.front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        World::camera.position -=  World::camera.front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        World::camera.position -=
            glm::normalize(glm::cross(World::camera.front, World::camera.up)) * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        World::camera.position +=
            glm::normalize(glm::cross(World::camera.front, World::camera.up)) * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        World::camera.position += World::camera.up * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        World::camera.position -= World::camera.up * velocity;
    }
}
