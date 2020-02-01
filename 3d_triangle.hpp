#ifndef TRIANGLE_3D
#define TRIANGLE_3D
#include "3d_vec3d.hpp"
//struct vec3d;

struct triangle
{
    vec3d p[3] = {0};
    vec3d col = vec3d(255);
    triangle()
    {
        p[0] = vec3d();
        p[1] = vec3d();
        p[2] = vec3d();
        vec3d col = vec3d(255);
    }
    triangle(vec3d _x, vec3d _y, vec3d _z)
    {
        p[0] = vec3d(_x);
        p[1] = vec3d(_y);
        p[2] = vec3d(_z);
        vec3d col = vec3d(255);
    }
    triangle(float _x0, float _y0, float _z0, 
             float _x1, float _y1, float _z1, 
             float _x2, float _y2, float _z2)
    {
        p[0] = vec3d(_x0, _y0, _z0);
        p[1] = vec3d(_x1, _y1, _z1);
        p[2] = vec3d(_x2, _y2, _z2);
        vec3d col = vec3d(255);
    }
    friend std::ostream& operator<<(std::ostream& os, const triangle& dt);
};
#endif