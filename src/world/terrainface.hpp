#ifndef TERRAIN_FACE_HPP
#define TERRAIN_FACE_HPP

#include <glm/glm.hpp>

#include "../util/mesh.hpp"
#include "../noise/noisefilter.hpp"

class ShapeSettings{
    public:
        float radius;
        NoiseFilter noiseFilter;
        ShapeSettings(float radius = 1.0f){this->radius = radius;}
        glm::vec3 CalculatePointOnPlanet(glm::vec3 pointOnUnitSphere){
            float elevation = noiseFilter.Evaluate(pointOnUnitSphere);
            return pointOnUnitSphere * radius * (1 + elevation);
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
