
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

mat4x4 matrixUnit()
{
    mat4x4 matRot;
    matRot.m[0][0] = 1;
    matRot.m[1][1] = 1;
    matRot.m[2][2] = 1;
    matRot.m[3][3] = 1;
    return matRot;
}
mat4x4 matrixRotationX(float fTheta)
{
    mat4x4 matRot;
    matRot.m[0][0] = 1;
    matRot.m[1][1] = cosf(fTheta);
    matRot.m[1][2] = sinf(fTheta);
    matRot.m[2][1] = -sinf(fTheta);
    matRot.m[2][2] = cosf(fTheta);
    matRot.m[3][3] = 1;
    return matRot;
}
mat4x4 matrixRotationY(float fTheta)
{
    mat4x4 matRot;
    matRot.m[0][0] = cosf(fTheta);
    matRot.m[1][1] = 1;
    matRot.m[0][2] = sinf(fTheta);
    matRot.m[2][0] = -sinf(fTheta);
    matRot.m[2][2] = cosf(fTheta);
    matRot.m[3][3] = 1;
    return matRot;
}
mat4x4 matrixRotationZ(float fTheta)
{
    mat4x4 matRot;
    matRot.m[0][0] = cosf(fTheta);
    matRot.m[0][1] = sinf(fTheta);
    matRot.m[1][0] = -sinf(fTheta);
    matRot.m[1][1] = cosf(fTheta);
    matRot.m[2][2] = 1;
    matRot.m[3][3] = 1;
    return matRot;
}
mat4x4 matrixMakeProjection(float fFovDeg, float fAspectRatio, float fFar, float fNear)
{
    mat4x4 matProj;

    float fFovRad = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);            // q
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear); //-zN q
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
    return matProj;
}
mat4x4 matrixTranslation(vec3d *t)
{
    mat4x4 matTrans;
    matTrans.m[0][0] = 1.0f;
    matTrans.m[1][1] = 1.0f;
    matTrans.m[2][2] = 1.0f;
    matTrans.m[3][3] = 1.0f;
    matTrans.m[3][0] = t->x;
    matTrans.m[3][1] = t->y;
    matTrans.m[3][2] = t->z;
    return matTrans;
}
mat4x4 matrixTranslation(float x, float y, float z)
{
    vec3d temp = vec3d(x,y,z);
    return matrixTranslation(&temp);
}