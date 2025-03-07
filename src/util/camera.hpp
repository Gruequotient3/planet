#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{

    public:
        float yaw;
        float pitch;
        float fov;

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;

        Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 3.0f), float _yaw = -90.0f,
               float _pitch = 0.0f, float _fov = 90.0f){
            position = _position;
            front = glm::vec3(0.0f, 0.0f, -1.0f);
            up = glm::vec3(0.0f, 1.0f, 0.0f);

            yaw = _yaw;
            pitch = _pitch;
            fov = _fov;

            view = glm::mat4(1.0f);
            view = glm::lookAt(position, position + front, up);
            view = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 10000.0f);

        }

        void Update(){
            view = glm::lookAt(position, position + front, up);
            projection = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 10000.0f);
        }

        GLfloat * GetViewMatrice(){
            return glm::value_ptr(view);
        }
        GLfloat * GetProjectionMatrice(){
            return glm::value_ptr(projection);
        }

    private:
        glm::mat4 view;
        glm::mat4 projection;


};




#endif
