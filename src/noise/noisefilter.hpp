#ifndef NOISE_FILTER_HPP
#define NOISE_FILTER_HPP

#include <glm/glm.hpp>

#include "noise.hpp"


class NoiseFilter{
    public:
        Noise noise;

        NoiseFilter() {};
        NoiseFilter(NoiseSettings &noiseSettings);
        float Evaluate(glm::vec3 point);


};


#endif
