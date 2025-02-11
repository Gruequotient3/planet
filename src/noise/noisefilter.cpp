#ifndef NOISE_FILTER_CPP
#define NOISE_FILTER_CPP


#include "noisefilter.hpp"


NoiseFilter::NoiseFilter(NoiseSettings &noiseSettings) : noise{Noise(noiseSettings)}
{}

float NoiseFilter::Evaluate(glm::vec3 point){

    float noiseValue = noise.perlinNoise3D(point.x, point.y, point.z);
    return noiseValue;
}

#endif
