#ifndef TERRAIN_FACE_HPP
#define TERRAIN_FACE_HPP

#include <glm/glm.hpp>

#include <vector>

#include "../util/mesh.hpp"
#include "../noise/noisefilter.hpp"

class ShapeSettings{
    public:
        float radius;
        std::vector<NoiseFilter> noiseFilter;
        ShapeSettings(float radius = 1.0f) :
            radius{radius}{}
        glm::vec3 CalculatePointOnPlanet(glm::vec3 pointOnUnitSphere){
            float firstLayerValue = 0.0f;
            float elevation = 0.0f;

            if (noiseFilter.size() > 0){
                firstLayerValue = noiseFilter[0].Evaluate(pointOnUnitSphere);
                if (noiseFilter[0].enable) elevation = firstLayerValue;
            }
            for (int i = 1; i < (int)noiseFilter.size(); ++i){
                if (noiseFilter[i].enable){
                    float mask = noiseFilter[i].useFirstLayerAsMask ? firstLayerValue : 1;
                    elevation += noiseFilter[i].Evaluate(pointOnUnitSphere) * mask;
                }
            }
            return pointOnUnitSphere * radius * (1+elevation);
        }

};

class TerrainFace{
    private:
        glm::vec3 axisA;
        glm::vec3 axisB;

    public:
        Mesh mesh;

        TerrainFace() {}
        TerrainFace(ShapeSettings &shapeSettings, int resolution, glm::vec3 localUp);
        void Initialize(int resolution, glm::vec3 localUp);
        void Initialize(int resolution, glm::vec3 localUp, ShapeSettings &shapeSettings);
        void ConstructMesh();

    private:
        int resolution;
        glm::vec3 localUp;
        ShapeSettings shapeSettings;


};



#endif
