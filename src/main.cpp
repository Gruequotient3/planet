#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "gfx/shader.hpp"
#include "gfx/texture.hpp"
#include "util/mesh.hpp"
#include "util/camera.hpp"
#include "util/direction.hpp"
#include "noise/noise.hpp"

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void UpdateDeltaTime();

void processInput(GLFWwindow* window);

float deltaTime = 0.0f;
float cameraSpeed = 4.0f;

bool firstMouse = true;
float lastX;
float lastY;

Camera camera = Camera();

const unsigned int tempindices[] = {0, 1, 3, 3, 1, 2};

const float tempvertices[] = {
    -1, -1, -1,
     1, -1, -1,
     1,  1, -1,
    -1,  1, -1,
};

int main(){

    // Initialise GLFW
    if (!glfwInit()){
        std::cout << "Failed to initialise GLFW" << std::endl;
        return -1;
    }


    // OpenGL context parameters
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialise OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1920, 1080, "Hello World !", NULL, NULL);

    if (!window){
        std::cout << "Failed to initialise OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);


    Shader shader = Shader("./res/shaders/shader.vert", "./res/shaders/shader.frag");

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i < sizeof(tempindices) / sizeof(unsigned int); ++i){
        indices.push_back(tempindices[i]);
    }
    for (int i = 0; i < sizeof(tempvertices) / sizeof(float) / 3; ++i){
        Vertex vertex;
        vertex.position = glm::vec3(tempvertices[i*3], tempvertices[i*3+1], tempvertices[i*3+2]);
        vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.texCoord = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    Mesh mesh {vertices, indices};

    // Hide Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);


    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", GL_FALSE, glm::value_ptr(model));
    // Main loop
    while(!glfwWindowShouldClose(window)){
        UpdateDeltaTime();
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        camera.Update();
        shader.use();

        model = glm::mat4(1.0f);
        shader.setMat4("model", GL_FALSE, glm::value_ptr(model));
        shader.setMat4("view", GL_FALSE, camera.GetViewMatrice());
        shader.setMat4("projection", GL_FALSE, camera.GetProjectionMatrice());

        mesh.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void frameBufferSizeCallBack(GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
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

void scrollCallback(GLFWwindow*, double, double yoffset){

    camera.fov -= (float)yoffset;
    if (camera.fov < 1.0f){
        camera.fov = 1.0f;
    }
    if (camera.fov > 90.0f){
        camera.fov = 90.0f;
    }
}


void UpdateDeltaTime(){
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.position +=  camera.front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.position -=  camera.front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.position += camera.up * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.position -= camera.up * velocity;
    }
}
