#include "3d_mat4x4.hpp"
#include "3d_vec3d.hpp"
#include <iostream>

int main()
{
vec3d Xvec = vec3d(1,0,0);
vec3d Yvec = vec3d(0,1,0);
vec3d Zvec = vec3d(0,0,1);

std::cout<<Xvec<<std::endl;

vec3d TESTvec = Xvec * matrixRotationU(&Yvec,3.141592f/2.0f);
vec3d TESTvec1 = TESTvec * matrixRotationU(&Xvec,3.141592f/2.0f);
std::cout<<TESTvec<<std::endl;
std::cout<<TESTvec1<<std::endl;
return 0;
}
