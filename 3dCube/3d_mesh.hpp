#ifndef MESH_3D
#define MESH_3D

#include <vector>

struct triangle;

struct mesh
{
    std::vector<triangle> tris;
};
#endif