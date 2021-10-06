#ifndef MAT4X4_3D
#define MAT4X4_3D
#include <strstream>
#include <cmath>

#include "3d_vec3d.hpp"
struct vec3d;

struct vec3d;
struct mat4x4
{
    float m[4][4] = {0};

    mat4x4 operator * (const mat4x4 &rhs);
    

    friend std::ostream& operator<<(std::ostream& os, const mat4x4& dt);
};

mat4x4 matrixUnit();
mat4x4 matrixRotationX(float fTheta);
mat4x4 matrixRotationY(float fTheta);
mat4x4 matrixRotationZ(float fTheta);
mat4x4 matrixRotationU(vec3d *U, float fTheta);
mat4x4 matrixMakeProjection(float fFovDeg, float fAspectRatio, float fFar, float fNear);
mat4x4 matrixTranslation(vec3d *t);
mat4x4 matrixTranslation(float x, float y, float z);
#endif