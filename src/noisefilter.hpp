#ifndef NOISE_FILTER_HPP
#define NOISE_FILTER_HPP

#include <glm/glm.hpp>

#include "noise.hpp"


enum NoiseFilterType {SIMPLE = 0, RIDGID};

class NoiseFilter{
    public:
        Noise noise;
        int type;
        bool enable;
        bool useFirstLayerAsMask;

        NoiseFilter() {};
        NoiseFilter(Noise noise, int filterType,
                    bool enable, bool useFirstLayerAsMask);
        float Evaluate(glm::vec3 point, glm::vec3 offset);
};


#endif
