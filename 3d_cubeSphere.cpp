#include "3d_cubeSphere.hpp"
#include "3d_terrainface.hpp"


std::vector<triangle> Cube::constructCube(std::vector<triangle> world)
{
    int resolution = 7;
    TerrainFace TF0 = TerrainFace(this->r, this->r, resolution, vec3d( 0, 0,-1), this->r / 2);
    world = TF0.constructMesh(world);
    TerrainFace TF1 = TerrainFace(this->r, this->r, resolution, vec3d( 0, 0, 1), this->r / 2);
    world = TF1.constructMesh(world);

    TerrainFace TF2 = TerrainFace(this->r, this->r, resolution, vec3d( 0,-1, 0), this->r / 2);
    world = TF2.constructMesh(world);
    TerrainFace TF3 = TerrainFace(this->r, this->r, resolution, vec3d( 0, 1, 0), this->r / 2);
    world = TF3.constructMesh(world);

    TerrainFace TF4 = TerrainFace(this->r, this->r, resolution, vec3d(-1, 0, 0), this->r / 2);
    world = TF4.constructMesh(world);
    TerrainFace TF5 = TerrainFace(this->r, this->r, resolution, vec3d( 1, 0, 0), this->r / 2);
    world = TF5.constructMesh(world);

    return world;
}