#ifndef NOISE_CPP
#define NOISE_CPP


#include <glad/glad.h>

#include "noise.hpp"

#include <cmath>
#include <iostream>

#define max(a, b) (a < b ? b : a)
#define lerp(a, b, t) (a + t * (b - a))
#define inverseLerp(a, b, t) ((t - a) / (b - a))

Noise::Noise(NoiseSettings &noiseSettings) : noiseSettings{noiseSettings}{};

float Noise::noise2D(float x, float y){
    int n = x + y * 257;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) &
    0x7fffffff) / 1073741824.0f);
}

float Noise::noise3D(float x, float y, float z){
    int n = x + y * 257 + z * 757;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) &
    0x7fffffff) / 1073741824.0f);
}

float Noise::lerpNoise2D(float x, float y){
    float xi, yi;
    float xf, yf;
    float l00, l01, l10, l11;
    float l0, l1;

    xi = x >= 0 ? (int) x : (int) x - 1;
    xf = x - (float) xi;

    yi = y >= 0 ? (int) y : (int) y - 1;
    yf = y - (float) yi;

    l00 = noise2D(xi, yi);
    l01 = noise2D(xi+1, yi);
    l10 = noise2D(xi, yi+1);
    l11 = noise2D(xi+1, yi+1);

    l0 = lerp(l00, l01, xf);
    l1 = lerp(l10, l11, xf);

    return lerp(l0, l1, yf);
}

float Noise::lerpNoise3D(float x, float y, float z){
    float xi, yi, zi;
    float xf, yf, zf;
    float l000, l001, l010, l011, l100, l101, l110, l111;
    float l00, l01, l10, l11;
    float l0, l1;

    xi = x >= 0 ? (int) x : (int) x - 1;
    xf = x - (float) xi;

    yi = y >= 0 ? (int) y : (int) y - 1;
    yf = y - (float) yi;

    zi = z >= 0 ? (int) z : (int) z - 1;
    zf = z - (float)zi;

    l000 = noise3D(xi, yi, zi);
    l001 = noise3D(xi+1, yi, zi);
    l010 = noise3D(xi, yi+1, zi);
    l011 = noise3D(xi+1, yi+1, zi);
    l100 = noise3D(xi, yi, zi+1);
    l101 = noise3D(xi+1, yi, zi+1);
    l110 = noise3D(xi, yi+1, zi+1);
    l111 = noise3D(xi+1, yi+1, zi+1);

    l00 = lerp(l000, l001, xf);
    l01 = lerp(l010, l011, xf);
    l10 = lerp(l100, l101, xf);
    l11 = lerp(l110, l111, xf);

    l0 = lerp(l00, l01, yf);
    l1 = lerp(l10, l11, yf);

    return lerp(l0, l1, zf);
}

float Noise::perlinNoise2D(glm::vec2 point, glm::vec2 offset){
    float noiseValue = 0;
    float frequency = noiseSettings.frequency;
    float amplitude = noiseSettings.amplitude;

    for (int i = 0; i < noiseSettings.octave; ++i){
        glm::vec2 pf = point * frequency + offset;
        float v = lerpNoise2D(pf.x, pf.y);
        noiseValue += (v + 1) * 0.5 * amplitude;
        frequency *= noiseSettings.lucanarity;
        amplitude *= noiseSettings.persistence;
    }
    return noiseValue * noiseSettings.strength;
}


float Noise::perlinNoise3D(glm::vec3 point, glm::vec3 offset){
    float noiseValue = 0;
    float frequency = noiseSettings.frequency;
    float amplitude = noiseSettings.amplitude;

    for (int i = 0; i < noiseSettings.octave; ++i){
        glm::vec3 pf = point * frequency + offset;
        float v = lerpNoise3D(pf.x, pf.y, pf.z);
        noiseValue += (v + 1) * 0.5f * amplitude;
        frequency *= noiseSettings.lucanarity;
        amplitude *= noiseSettings.persistence;
    }
    noiseValue = max(0, noiseValue - noiseSettings.minValue);
    return noiseValue * noiseSettings.strength;
}

float Noise::ridgidPerlinNoise3D(glm::vec3 point, glm::vec3 offset){
    float noiseValue = 0.0f;
    float frequency = noiseSettings.frequency;
    float amplitude = noiseSettings.amplitude;
    float weight = 1.0f;

    for (int i = 0; i < noiseSettings.octave; ++i){
        glm::vec3 pf = point * frequency + offset;
        float v = 1.0f-abs(lerpNoise3D(pf.x, pf.y, pf.z));
        v *= v;
        v *= weight;
        weight = v;

        noiseValue += v * amplitude;
        frequency *= noiseSettings.lucanarity;
        amplitude *= noiseSettings.persistence;
    }
    noiseValue = max(0.0f, noiseValue - noiseSettings.minValue);
    return noiseValue * noiseSettings.strength;
}



#endif
