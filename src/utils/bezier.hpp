#ifndef BEZIER_HPP
#define BEZIER_HPP

#include <vector>

#include <glm/glm.hpp>

class Bezier{
    private:
        int Binomial(int n, int k);
        int Factorial(int n);
    public:
        std::vector<glm::vec3> points;
        
        glm::vec3 Calculate(float t);
};

#endif