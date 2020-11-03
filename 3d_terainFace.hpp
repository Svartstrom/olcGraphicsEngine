
#ifndef CAMERA_3D
#define CAMERA_3D

#include "3d_vec3d.hpp"
#include "3d_triangle.hpp"
#include <vector>

//#include "olcPixelGameEngine.h"


struct terainFace
{
    public:
    private:
    int resolution;
    vec3d localUp;
    std::vector<triangle> mesh;
    terainFace(vec3d localUp, int resolution)
    {
        this->localUp = localUp;
        this->resolution = resolution;

    }
};
#endif