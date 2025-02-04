#ifndef NOISE_HPP
#define NOISE_HPP

#include <glm/glm.hpp>

#include "../gfx/texture.hpp"


class Noise{
    public:
        Noise(int _width, int _height, int _octave = 1, float amplitude = 1.0f,
              float _frequency = 1.0f, float _persistence = 1.0f, float lucanarity = 1.0f,
              float offsetx = 0.0f, float offsety = 0.0f, int seed = 0);
        int width, height;
        int octave;
        float amplitude;
        float frequency;
        float persistence;
        float lucanarity;
        float offsetx;
        float offsety;
        int seed;

        float* generatePerlinNoiseMap();

        Texture getPerlinNoiseMapTexture();
    private:
        float noise2D(int x, int y);
        float lerpNoise2D(float x, float y);
        float perlinNoise2D(float x, float y);
        unsigned int* getNoiseMapColor(float* noise);

};



#endif
