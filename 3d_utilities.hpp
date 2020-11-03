#ifndef UTILITIES_3D
#define UTILITIES_3D
#include <strstream>
#include <cmath>

#include "3d_mat4x4.hpp"
#include "3d_vec3d.hpp"

mat4x4 matrixUnit(void);
mat4x4 matrixRotationX(float fTheta);
mat4x4 matrixRotationY(float fTheta);
mat4x4 matrixRotationZ(float fTheta);
mat4x4 matrixTranslation(vec3d t);
mat4x4 matrixTranslation(float x, float y, float z);
mat4x4 matrixMakeProjection(float fFovDeg, float fAspectRatio, float fFar, float fNear);
mat4x4 matrixPointAt(vec3d & pos, vec3d & newForward, vec3d & newUp, vec3d & newRight);
mat4x4 matrixLookAt(vec3d & pos, vec3d & newForward, vec3d & newUp, vec3d & newRight);
mat4x4 PointAt(vec3d & pos, vec3d & target, vec3d & up);

#endif