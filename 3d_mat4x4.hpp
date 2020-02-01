#ifndef MAT4X4_3D
#define MAT4X4_3D
#include <strstream>
#include "3d_mat4x4.hpp"

struct mat4x4
{
    float m[4][4] = {0};

    mat4x4 operator * (const mat4x4 &rhs);

    friend std::ostream& operator<<(std::ostream& os, const mat4x4& dt);
};
#endif