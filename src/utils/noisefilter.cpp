#ifndef NOISE_FILTER_CPP
#define NOISE_FILTER_CPP


#include "noisefilter.hpp"

NoiseFilter::NoiseFilter(Noise noise, int filterType,
                         bool enable, bool useFirstLayerAsMask) :
    noise{noise}, type{(int)filterType},
    enable{enable}, useFirstLayerAsMask{useFirstLayerAsMask}{}

float NoiseFilter::Evaluate(glm::vec3 point, glm::vec3 offset){
    float noiseValue = 0;
    switch(type){
        case SIMPLE:
            noiseValue = noise.perlinNoise3D(point, offset);
            break;
        case RIDGID:
            noiseValue = noise.ridgidPerlinNoise3D(point, offset);
            break;
    }
    return noiseValue;
}

#endif
