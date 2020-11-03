#include "3d_utilities.hpp"

mat4x4 matrixUnit(void)
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

mat4x4 matrixTranslation(vec3d t)
{
    mat4x4 matTrans;
    matTrans.m[0][0] = 1.0f;
    matTrans.m[1][1] = 1.0f;
    matTrans.m[2][2] = 1.0f;
    matTrans.m[3][3] = 1.0f;
    matTrans.m[3][0] = t.x;
    matTrans.m[3][1] = t.y;
    matTrans.m[3][2] = t.z;
    return matTrans;
}

mat4x4 matrixTranslation(float x, float y, float z)
{
    return matrixTranslation(vec3d(x,y,z));
}

mat4x4 matrixMakeProjection(float fFovDeg, float fAspectRatio, float fFar, float fNear)
{
    mat4x4 matProj;

    float fFovRad   = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);            // q
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear); //-zN q
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    return matProj;
}

mat4x4 matrixPointAt(vec3d & pos, vec3d & newForward, vec3d & newUp, vec3d & newRight)
{
    mat4x4 matrix;
    matrix.m[0][0] = newRight.x;   matrix.m[0][1] = newRight.y;   matrix.m[0][2] = newRight.z;   matrix.m[0][3] = 0;
    matrix.m[1][0] = newUp.x;      matrix.m[1][1] = newUp.y;      matrix.m[1][2] = newUp.z;      matrix.m[1][3] = 0;
    matrix.m[2][0] = newForward.x; matrix.m[2][1] = newForward.y; matrix.m[2][2] = newForward.z; matrix.m[2][3] = 0;
    matrix.m[3][0] = pos.x;        matrix.m[3][1] = pos.y;        matrix.m[3][2] = pos.z;        matrix.m[3][3] = 1;
    return matrix;
}

mat4x4 matrixLookAt(vec3d & pos, vec3d & newForward, vec3d & newUp, vec3d & newRight)
{
    mat4x4 matrix;
    matrix.m[0][0] = newRight.x;   matrix.m[1][0] = newRight.y;   matrix.m[2][0] = newRight.z;   matrix.m[0][3] = 0;
    matrix.m[0][1] = newUp.x;      matrix.m[1][1] = newUp.y;      matrix.m[2][1] = newUp.z;      matrix.m[1][3] = 0;
    matrix.m[0][2] = newForward.x; matrix.m[1][2] = newForward.y; matrix.m[2][2] = newForward.z; matrix.m[2][3] = 0;
    matrix.m[3][0] = (newRight*pos)*-1; matrix.m[3][1] = (newUp*pos)*-1;    matrix.m[3][2] = (newForward*pos)*-1; matrix.m[3][3] = 1;
    return matrix;
}

mat4x4 PointAt(vec3d & pos, vec3d & target, vec3d & up)
{
    vec3d newForward = (target - pos).normal();
    vec3d newUp = (up - (newForward * up.dot(newForward))).normal();
    vec3d newRight = newUp.cross(newForward);

    return matrixLookAt(pos, newForward, newUp, newRight);
}