#ifndef PLANET_HPP
#define PLANET_HPP

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/shader.hpp"
#include "../util/mesh.hpp"
#include "../util/data.hpp"
#include "../noise/noise.hpp"
#include "../noise/noisefilter.hpp"
#include "terrainface.hpp"

#define TERRAINFACES 6

class Planet{
    public:
        int resolution;

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
        TerrainFace terrainFace[TERRAINFACES];
        void Initialize();
};


#endif
