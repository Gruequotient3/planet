#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/shader.hpp"
#include "../utils/camera.hpp"
#include "../utils/data.hpp"
#include "../utils/noise.hpp"
#include "../utils/noisefilter.hpp"
#include "terrainface.hpp"

#define TERRAINFACES 6

enum PlanetType {PLANET, SATELITE, SUN};

struct ColorSettings{
    std::vector<float> colors;
    std::vector<float> heights;
};

class Planet{
    public:
        int resolution;
        glm::vec3 position;
        float scale;

        // Simulation parameters
        PlanetType type;
        float rotationSpeed;
        float revolutionSpeed;

        Planet();
        Planet(const Planet &pt);

        Planet &operator=(const Planet &pt);
        
        void Destroy();
        void Draw(Shader &shader);
        void SetSettingsFromData(Data &data);
        void SetColorsFromData(Data &data);
        void SetMinMax();
        
        ColorSettings colorSettings;
    private:
        ShapeSettings shapeSettings;
        
        float min, max;
        
        TerrainFace terrainFace[TERRAINFACES];
        void Init();
        void GenerateMesh();
};


#endif
