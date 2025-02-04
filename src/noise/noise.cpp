#ifndef NOISE_CPP
#define NOISE_CPP


#include <glad/glad.h>

#include "noise.hpp"

#include <cmath>
#include <iostream>
#include <stdio.h>

#define lerp(a, b, t) (a + t * (b - a))
#define inverseLerp(a, b, t) ((t - a) / (b - a))

Noise::Noise(int _width, int _height, int _octave, float _amplitude,
             float _frequency, float _persistence, float _lucanarity,
             float _offsetx, float _offsety, int _seed){

    width = _width; height = _height;
    octave = _octave;
    amplitude = _amplitude;
    frequency = _frequency;
    persistence = _persistence;
    lucanarity = _lucanarity;
    offsetx = _offsetx;
    offsety = _offsety;
    seed = _seed;
}

float Noise::noise2D( int x, int y ){
    int n = x + y * 257;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) &
    0x7fffffff) / 1073741824.0);
}


float Noise::lerpNoise2D(float x, float y){
    int xi, yi;
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

float Noise::perlinNoise2D(float x, float y){
    float r = 0.0f;
    float f = frequency;
    float amplitude = 1.0f;
    float dx = x, dy = y;
    for(int i = 0; i < octave; i++)
    {
        r += lerpNoise2D(dx * f, dy * f) * amplitude;
        dx *= lucanarity;
        dy *= lucanarity;
        amplitude *= persistence;
    }
    if (amplitude == 1.0f)
        amplitude += 0.0000001;
    float geoLim = (1 - persistence) / (1 - amplitude);
    return (r * geoLim + 1.0f) / 2.0f;
}

float* Noise::generatePerlinNoiseMap(){
    srand(seed);
    float randOffsetx = rand() % 20000 - 10000;
    float randOffsety = rand() % 20000 - 10000;

    float* noise = new float [width * height];
    for (int y = 0; y < height; ++y){
        int dy = y + offsety + randOffsety;
        for (int x = 0; x < width; ++x){
            int dx = x + offsetx + randOffsetx;
            float noiseValue = (perlinNoise2D((float)dx, (float) dy));
            noise[y * width + x] = noiseValue;
        }
    }

    for (int i = 0, wh = width * height; i < wh; ++i){
        noise[i] = inverseLerp(0, 1, noise[i]);
    }
    return noise;
}

Texture Noise::getPerlinNoiseMapTexture(){
    float* noise = generatePerlinNoiseMap();
    unsigned int* image = getNoiseMapColor(noise);

    Texture noiseTexture = Texture(image, width, height);
    delete image;
    return noiseTexture;
}



unsigned int* Noise::getNoiseMapColor(float* noise){
    unsigned int* image = new unsigned int[width * height];
    for (int i = 0, wh = width * height; i < wh; ++i){
        unsigned char noiseValue = (unsigned char)lerp(0.0, 255.0, noise[i]);
        image[i] = (((GLuint)noiseValue) << 24 | ((GLuint)noiseValue) << 16 |
                 ((GLuint)noiseValue) << 8 | ((GLuint)255) << 0);
    }
    return image;
}



#endif
