#include "3d_terrainface.hpp"

vec3d TerrainFace::coordFromIndexFlat(int index)
{
    vec3d localCenter = this->localUp * this->dist;
    vec3d deltaA = (this->axisA * (this->width  / 2.0f));
    vec3d deltaB = (this->axisB * (this->height / 2.0f));
    vec3d topLeft = localCenter - deltaA - deltaB;
    float pA = (index%this->resolution)/(float)(this->resolution-1);
    float pB = (index/this->resolution)/(float)(this->resolution-1);
    vec3d Adir = (this->axisA * pA * this->width);
    vec3d Bdir = (this->axisB * pB * this->height);
    vec3d targetNode = topLeft + Adir + Bdir;
    return targetNode;
}
vec3d TerrainFace::coordFromIndexSphere(int index)
{
    vec3d localCenter = this->localUp * this->dist;
    vec3d flatVec = coordFromIndexFlat(index);
    vec3d retVec = flatVec * (localCenter.abs() / flatVec.abs());
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