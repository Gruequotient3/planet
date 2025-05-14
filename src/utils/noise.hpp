#ifndef NOISE_HPP
#define NOISE_HPP

#include <glm/glm.hpp>

struct NoiseSettings{
    float strength ;
    int octave;
    float amplitude;
    float frequency;
    float persistence;
    float lucanarity;
    float minValue;
};

class Noise{
    public:
        NoiseSettings noiseSettings;

        Noise();
        Noise(NoiseSettings &noiseSettings);

        float noise3D(float x, float y, float z);
        float lerpNoise3D(float x, float y, float z);
        float perlinNoise3D(glm::vec3 point, glm::vec3 offset);
        float ridgidPerlinNoise3D(glm::vec3 point, glm::vec3 offset);
};



#endif
