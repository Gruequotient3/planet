#ifndef NOISE_HPP
#define NOISE_HPP

#include <glm/glm.hpp>

#include "../gfx/texture.hpp"

struct NoiseSettings{
    float strength = 1.0f;
    int octave = 1;
    float amplitude = 1.0;
    float frequency = 1.0;
    float persistence = 1.0;
    float lucanarity = 1.0;
    float minValue = 0.0;
};

class Noise{
    public:
        NoiseSettings noiseSettings;

        Noise() {};
        Noise(NoiseSettings &noiseSettings);

        float noise2D(float x, float y);
        float noise3D(float x, float y, float z);
        float lerpNoise2D(float x, float y);
        float lerpNoise3D(float x, float y, float z);
        float perlinNoise2D(glm::vec2 point, glm::vec2 offset);
        float perlinNoise3D(glm::vec3 point, glm::vec3 offset);
        float ridgidPerlinNoise3D(glm::vec3 point, glm::vec3 offset);
};



#endif
