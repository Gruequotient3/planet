#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera{
    private:
        glm::mat4 view;
        glm::mat4 perspective;
        glm::mat4 orthographic;

    public:
        float ratio;
        float width;
        int height;

        float yaw;
        float pitch;
        float fov;

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;

        Camera(float ratio, int width, int height);

        void Update();

        float *GetViewMatrice();
        float *GetPerspectiveMatrice();
        float *GetOrthogaphicMatrice();
};

#endif
