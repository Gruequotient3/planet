#ifndef BEZIER_HPP
#define BEZIER_HPP

#include <vector>

#include <glm/glm.hpp>

class Bezier{
    private:
        int binomial(int n, int k);
        int factorial(int n);
    public:
        std::vector<glm::vec3> points;
        
        glm::vec3 Calculate(float t);
};

#endif