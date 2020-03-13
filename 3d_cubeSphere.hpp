#ifndef CUBESPHERE_3D
#define CUBESPHERE_3D

#include "3d_vec3d.hpp"
#include "3d_mesh.hpp"
#include "3d_triangle.hpp"
#include <vector>
#include <cmath>

#define PI 3.141592026F

struct TerrainFace
{
public:
    TerrainFace(float width, float height, int resolution, vec3d localUp, float dist)
    {
        this->width = width;
        this->height = height;
        this->dist = dist;
        this->rad = dist;
        this->resolution = resolution;
        this->localUp = localUp;

        axisA = vec3d(localUp.y, localUp.z, localUp.x);
        axisB = localUp.cross(axisA);
    }

    std::vector<triangle> constructMesh(std::vector<triangle> world);
    
private:
    int resolution;
    float width;
    float height;
    float dist;
    float rad;
    vec3d localUp;
    vec3d axisA;
    vec3d axisB;

    vec3d coordFromIndexFlat(int index);
    vec3d coordFromIndexSphere(int index);
};

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

vec3d TerrainFace::coordFromIndexFlat(int index)
{
    vec3d localCenter = this->localUp * this->dist;
    vec3d deltaA = (this->axisA * (this->width  / 2.0f));
    vec3d deltaB = (this->axisB * (this->height / 2.0f));
    vec3d topLeft = localCenter - deltaA - deltaB;
    float pX = (index%this->resolution)/(float)(this->resolution-1);
    float pY = (index/this->resolution)/(float)(this->resolution-1);
    vec3d Adir = (this->axisA * pX * this->width);
    vec3d Bdir = (this->axisB * pY * this->height);
    vec3d targetNode = topLeft + Adir + Bdir;
    vec3d deltaVec = localCenter - targetNode;
    return targetNode;
}
vec3d TerrainFace::coordFromIndexSphere(int index)
{
    vec3d topLeft = this->localUp * this->dist + this->axisA * this->width * 0.5f + this->axisB * this->height * 0.5f;
    float deltaAng = PI / (2.0f * (float)this->resolution-1);

    vec3d retVec = topLeft * matrixRotationU(&this->axisA, (index%this->resolution)*deltaAng);
    retVec = retVec * matrixRotationU(&this->axisB, (index/this->resolution)*deltaAng);

    return retVec;
}


std::vector<triangle> TerrainFace::constructMesh(std::vector<triangle> world)
{
    for (int y = 0; y < this->resolution; y++)
    {
        for (int x = 0; x < this->resolution; x++)
        {
            int i = x + y * this->resolution;

            if ( x != this->resolution-1 && y != this->resolution-1)
            {
                world.push_back(
                    triangle(coordFromIndexSphere(i), 
                             coordFromIndexSphere(i + this->resolution + 1), 
                             coordFromIndexSphere(i + this->resolution))
                    );
                world.push_back(
                    triangle(coordFromIndexSphere(i),
                             coordFromIndexSphere(i + 1),
                             coordFromIndexSphere(i + this->resolution + 1))
                    );
            }
        }
    }  
    return world;
}

std::vector<triangle> Cube::constructCube(std::vector<triangle> world)
{
    TerrainFace TF0 = TerrainFace(this->r, this->r, 7, vec3d( 0, 0,-1), this->r / 2);
    world = TF0.constructMesh(world);
    TerrainFace TF1 = TerrainFace(this->r, this->r, 7, vec3d( 0, 0, 1), this->r / 2);
    world = TF1.constructMesh(world);

    TerrainFace TF2 = TerrainFace(this->r, this->r, 7, vec3d( 0,-1, 0), this->r / 2);
    world = TF2.constructMesh(world);
    TerrainFace TF3 = TerrainFace(this->r, this->r, 7, vec3d( 0, 1, 0), this->r / 2);
    world = TF3.constructMesh(world);

    TerrainFace TF4 = TerrainFace(this->r, this->r, 7, vec3d(-1, 0, 0), this->r / 2);
    world = TF4.constructMesh(world);
    TerrainFace TF5 = TerrainFace(this->r, this->r, 7, vec3d( 1, 0, 0), this->r / 2);
    world = TF5.constructMesh(world);

    return world;
}
#endif