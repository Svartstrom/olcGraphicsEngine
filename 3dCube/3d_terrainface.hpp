#ifndef TERRAINFACE_3D
#define TERRAINFACE_3D
#include "3d_mat4x4.hpp"
#include "3d_vec3d.hpp"
#include "3d_triangle.hpp"

#include <vector>
#include <cmath>


struct TerrainFace
{
public:
    TerrainFace(float width, float height, int resolution, vec3d localUp, float dist)
    {
        this->width = width;
        this->height = height;
        this->dist = dist;
        this->rad = dist;
        this->resolution = resolution;
        this->localUp = localUp;

        axisA = vec3d(localUp.y, localUp.z, localUp.x).normal();
        axisB = localUp.cross(axisA).normal();
    }

    std::vector<triangle> constructMesh(std::vector<triangle> world);
    
private:
    int resolution;
    float width;
    float height;
    float dist;
    float rad;
    vec3d localUp;
    vec3d axisA;
    vec3d axisB;

    vec3d coordFromIndexFlat(int index);
    vec3d coordFromIndexSphere(int index);
};
#endif