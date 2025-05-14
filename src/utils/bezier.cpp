#ifndef BEZIER_CPP
#define BEZIER_CPP

#include <cmath>

#include "bezier.hpp"

int Bezier::Factorial(int n){
    int fact = 1;
    for (int  i = 2; i <= n; ++i){
        fact *= i;
    }
    return fact;
}

int Bezier::Binomial(int n, int k){
    return Factorial(n) / (Factorial(k) * Factorial(n-k));
}

glm::vec3 Bezier::Calculate(float t){
    glm::vec3 sum{0.0f};
    for (int i = 0, n = (int)points.size()-1; i <= n; ++i){
        sum += points[i] * (float)Binomial(n, i) * (float)pow(1-t, n-i) * (float)pow(t, i);
    }
    return sum;
}


#endif