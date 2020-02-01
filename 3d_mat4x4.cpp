
#include "3d_mat4x4.hpp"



std::ostream& operator<<(std::  ostream& os, const mat4x4& dt)
{
    for(int i = 0;i<4;i++)
    {
        for(int j =0;j<4;j++)
        {
            os << dt.m[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

mat4x4 mat4x4::operator * (const mat4x4 &rhs) 
{
    mat4x4 out;

    for(int r = 0;r<4;r++)
    {
        for(int c =0;c<4;c++)
        {
            out.m[r][c] = this->m[r][0] * rhs.m[0][c] + this->m[r][1] * rhs.m[1][c] + this->m[r][2] * rhs.m[2][c] + this->m[r][3] * rhs.m[3][c];
        }
    }
    return out;
}