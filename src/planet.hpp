#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "data.hpp"
#include "noise.hpp"
#include "noisefilter.hpp"
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

        Planet(Shader &shader);

        void GenerateMesh();
        void Draw();
        void SetSettingsFromData(Data &data);
        void SetColorsFromData(Data &data);
        void SetShader(Shader &shader);
        void SetMinMax();

        ColorSettings colorSettings;
    private:
        Shader &shader;

        ShapeSettings shapeSettings;

        float min, max;

        TerrainFace terrainFace[TERRAINFACES];

        void Initialize();
};


#endif
