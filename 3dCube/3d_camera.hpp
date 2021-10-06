#ifndef CAMERA_3D
#define CAMERA_3D

#include "3d_vec3d.hpp"
#include "olcPixelGameEngine.h"

struct camera
{
    vec3d pos = vec3d(0,0,0);
    vec3d dir = vec3d(1,0,0);

    camera()
    {
        pos = vec3d(0,0,0);
        dir = vec3d(1,0,0);
    }
    camera(vec3d _x, vec3d _y)
    {
        pos = vec3d(_x);
        dir = vec3d(_y);
    }
    void move(float fElapsedTime);
    void rotate(float fElapsedTime);
};

#endif