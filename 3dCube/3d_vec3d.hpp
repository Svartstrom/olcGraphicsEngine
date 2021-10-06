#ifndef VEC3D_3D
#define VEC3D_3D
#include "3d_mat4x4.hpp"
#include <cmath>
struct mat4x4;

struct vec3d
{
    float x, y, z, w;
    vec3d() : x(0), y(0), z(0) {}
    vec3d(int _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1) {}
    vec3d(int _x, int _y, int _z) : x((float)_x), y((float)_y), z((float)_z), w(1) {}

    vec3d operator + (const vec3d &rhs);
    vec3d operator - (const vec3d &rhs);
    vec3d operator / (const float rhs);
    vec3d operator * (const int rhs);
    vec3d operator * (const float rhs);
    float operator * (const vec3d &rhs);
    vec3d operator * (const mat4x4 &rhs);
    
    vec3d cross(const vec3d &rhs);
    vec3d normal(void);
    float dot(const vec3d &rhs);
    float abs();

    friend std::ostream& operator<<(std::ostream& os, const vec3d& dt);
};

vec3d Vector_IntersectPlane(vec3d & plane_p, vec3d &plane_n, vec3d &line_start, vec3d &line_end);
#endif