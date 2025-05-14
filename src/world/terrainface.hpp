#ifndef TERRAIN_FACE_HPP
#define TERRAIN_FACE_HPP

#include <glm/glm.hpp>

#include <cmath>
#include <vector>

#include "../utils/mesh.hpp"
#include "../utils/noisefilter.hpp"

class ShapeSettings{
    public:
        float radius;
        int seed;
        glm::vec3 offset;

        std::vector<NoiseFilter> noiseFilter;

        ShapeSettings(float radius = 1.0f, int seed = 0) : radius{radius}{
            SetSeed(seed);
        }

        glm::vec3 CalculatePointOnPlanet(glm::vec3 pointOnUnitSphere){
            float firstLayerValue = 0.0f;
            float elevation = 0.0f;

            if (noiseFilter.size() > 0){
                firstLayerValue = noiseFilter[0].Evaluate(pointOnUnitSphere, offset);
                if (noiseFilter[0].enable) elevation = firstLayerValue;
            }
            for (int i = 1; i < (int)noiseFilter.size(); ++i){
                if (noiseFilter[i].enable){
                    float mask = noiseFilter[i].useFirstLayerAsMask ? firstLayerValue : 1;
                    elevation += noiseFilter[i].Evaluate(pointOnUnitSphere, offset) * mask;
                }
            }
            return pointOnUnitSphere * radius * (1+elevation);
        }

        void SetSeed(int seed){
            this->seed = seed;
            srand(seed);
            offset = glm::vec3(rand() % 20000 - 10000,
                               rand() % 20000 - 10000,
                               rand() % 20000 - 10000);
        }
};

class TerrainFace{
    private:
        glm::vec3 axisA;
        glm::vec3 axisB;

        int resolution;
        glm::vec3 localUp;
        ShapeSettings shapeSettings;

    public:
        Mesh mesh;

        TerrainFace();
        void Init(int resolution, glm::vec3 localUp, ShapeSettings &shapeSettings);
        void Destroy();
        void GenerateMesh();
        void Draw(Shader &shader);
};



#endif
