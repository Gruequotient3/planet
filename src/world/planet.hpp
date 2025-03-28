#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/shader.hpp"
#include "../util/camera.hpp"
#include "../util/data.hpp"
#include "../noise/noise.hpp"
#include "../noise/noisefilter.hpp"
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

        // Simulation parameters
        PlanetType type;
        float rotationSpeed;
        float revolutionSpeed;

        Planet(Shader &shader);

        void GenerateMesh();
        void Draw();
        void SetSettingsFromData(Data &data);
        void SetColorsFromData(Data &data);
        void SetShader(Shader &shader);
        void SetMinMax();

    private:
        Shader &shader;

        ShapeSettings shapeSettings;
        ColorSettings colorSettings;

        float min, max;

        TerrainFace terrainFace[TERRAINFACES];

        void Initialize();
};


#endif
