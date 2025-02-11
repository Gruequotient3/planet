#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

#include <vector>

#include "../util/mesh.hpp"
#include "../noise/noise.hpp"
#include "../noise/noisefilter.hpp"
#include "terrainface.hpp"

#define TERRAINFACES 6

class Planet{
    public:
        int resolution;
        Planet(int resolution, float radius);
        Planet(NoiseSettings noiseSettings, int resolution, float radius);
        void GenerateMesh();
        void Draw(Shader& shader);

    private:
        ShapeSettings shapeSettings;
        TerrainFace terrainFace[TERRAINFACES];
        void Initialize();
};


#endif
