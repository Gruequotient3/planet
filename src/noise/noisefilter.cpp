#ifndef NOISE_FILTER_CPP
#define NOISE_FILTER_CPP


#include "noisefilter.hpp"

NoiseFilter::NoiseFilter(NoiseSettings &noiseSettings, int filterType,
                         bool enable, bool useFirstLayerAsMask) :
    noise{Noise(noiseSettings)}, type{(int)filterType},
    enable{enable}, useFirstLayerAsMask{useFirstLayerAsMask}{}

float NoiseFilter::Evaluate(glm::vec3 point){
    float noiseValue = 0;
    switch(type){
        case SIMPLE:
            noiseValue = noise.perlinNoise3D(point.x, point.y, point.z);
            break;
        case RIDGID:
            noiseValue = noise.ridgidPerlinNoise3D(point.x, point.y, point.z);

            break;
    }
    return noiseValue;
}

#endif
