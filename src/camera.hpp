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

        float ratio;

        Camera(float ratio = 16.0f / 9.0f,
                glm::vec3 position = glm::vec3{0.0f, 0.0f, 10.0f}, float yaw = -90.0f,
                float pitch = 0.0f, float fov = 90.0f){

            this->ratio = ratio;
            this->position = position;
            this->front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
            up = glm::vec3(0.0f, 1.0f, 0.0f);

            this->yaw = yaw;
            this->pitch = pitch;
            this->fov = fov;
            
            Update();
        }

        void Update(){
            view = glm::lookAt(position, position + front, up);
            projection = glm::perspective(glm::radians(fov),
                                          ratio, 0.1f, 10000.0f);
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
