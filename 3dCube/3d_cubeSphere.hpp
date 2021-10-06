#ifndef CUBESPHERE_3D
#define CUBESPHERE_3D

#include "3d_vec3d.hpp"
#include "3d_mesh.hpp"
#include "3d_triangle.hpp"
#include "3d_terrainface.hpp"

#include <vector>
#include <cmath>

#define PI 3.141592026F

struct Cube
{
public:
    Cube(float X_, float Y_, float R_)
    {
        this->x = X_;
        this->x = Y_;
        this->r = R_;
    }

    std::vector<triangle> constructCube(std::vector<triangle> world);

private:
    float x;
    float y;
    float r;
};
#endif