#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float ratio, int width, int height){
    this->ratio = ratio;
    this->width = width;
    this->height = height;
    yaw = -90.0;
    pitch = 0.0f;
    fov = 90.0f;

    position = glm::vec3{0.0f, 0.0f, 5.0f};
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    Update();
}

void Camera::Update(){
    view = glm::lookAt(position, position + front, up);
    perspective = glm::perspective(glm::radians(fov), ratio, 0.01f, 10000.0f);
    orthographic = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -1000.0f, 1000.0f);
}

float *Camera::GetViewMatrice(){
    return glm::value_ptr(view);
}

float *Camera::GetPerspectiveMatrice(){
    return glm::value_ptr(perspective);
}

float *Camera::GetOrthogaphicMatrice(){
    return glm::value_ptr(orthographic);
}



#endif
