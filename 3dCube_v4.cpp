
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
//#include "3dCube_structures.h"
#include <fstream>
#include <strstream>
#include <algorithm>

using namespace std;

struct vec3d
{
    float x, y, z, w;
    vec3d() : x(0), y(0), z(0) {}
    vec3d(int _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1) {}
    vec3d(int _x, int _y, int _z) : x((float)_x), y((float)_y), z((float)_z), w(1) {}

};

struct triangle
{
    vec3d p[3] = {0};
    triangle()
    {
        cout<<"empty"<<endl;
        p[0] = vec3d();
        p[1] = vec3d();
        p[2] = vec3d();
        int dp = 1;
    }
    triangle(vec3d _x, vec3d _y, vec3d _z)
    {
        cout<<"with 3"<<endl;
        p[0] = vec3d(_x);
        p[1] = vec3d(_y);
        p[2] = vec3d(_z);
        int dp = 0;
    }
    triangle(float _x0, float _y0, float _z0, 
             float _x1, float _y1, float _z1, 
             float _x2, float _y2, float _z2)
    {
        cout<<"longest"<<endl;
        cout<<_x0<<_y0<<_z0<<endl;
        p[0] = vec3d(_x0, _y0, _z0);
        p[1] = vec3d(_x1, _y1, _z1);
        p[2] = vec3d(_x2, _y2, _z2);
        int dp = 1;
    }
};

struct mesh
{
    vector<triangle> tris;
};

struct mat4x4
{
    float m[4][4] = {0};

        vec3d operator*(const vec3d &i)
    {

    }
};

class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Lines";
    }

private:
    mesh meshCube;
    mat4x4 matProj;
    vec3d vCamera = {0, 0, 0};
    mat4x4 matRotZ, matRotY, matRotX;
    float fTheta = 0;
    float fNear = 0.1f;
    float fFar = 100.0f;
    float fFovDeg = 90.0f;
    float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
    float fFovRad = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
    float bias = 0.5f;
    triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

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
        float fFovRad = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
        mat4x4 matProj;
        matProj.m[0][0] = fAspectRatio * fFovRad;
        matProj.m[1][1] = fFovRad;
        matProj.m[2][2] = fFar / (fFar - fNear);            // q
        matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear); //-zN q
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;
        return matProj;
    }

    void multiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }

    void normalizeVector(vec3d &input, vec3d &output)
    {
        float l = sqrtf(input.x * input.x + input.y * input.y + input.z * input.z);
        output.x = input.x / l;
        output.y = input.y / l;
        output.z = input.z / l;
    }

    float dotProduct(vec3d &input1, vec3d &input2, vec3d &camera)
    {
        float D = (input1.x - camera.x) * input2.x +
                  (input1.y - camera.y) * input2.y +
                  (input1.z - camera.z) * input2.z;
        return D;
    }

public:
    bool OnUserCreate() override
    {

        meshCube.tris = {
            //SOUTH
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

            //EAST
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

            //NORTH
            {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

            //WEST
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

            //TOP
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

            //BOTTOM
            {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        };

        matProj = matrixMakeProjection(fFovDeg, fAspectRatio, fFar, fNear);
        cout<<"end"<<endl;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        // Clear Screen
        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

        fTheta += 1.0f * fElapsedTime;
        
        matRotX = matrixRotationX(fTheta * bias);
        matRotY = matrixRotationY(fTheta);
        matRotZ = matrixRotationZ(fTheta);

        for (auto tri : meshCube.tris)
        {
            
            for (int i = 0; i < 3; i++)
            {
                multiplyMatrixVector(tri.p[i], triRotatedZ.p[i], matRotZ);
                multiplyMatrixVector(triRotatedZ.p[i], triRotatedZX.p[i], matRotY);

                triTranslated.p[i] = triRotatedZX.p[i];
                triTranslated.p[i].z = triRotatedZX.p[i].z + 30.0f;
            }
            vec3d normal, line1, line2;
            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            normalizeVector(normal, normal);

            float D = dotProduct(triTranslated.p[0], normal, vCamera);
            if (D < 0.0)
            {
                vec3d lightDirection = {0.0f, 0.0f, -1.0f};
                normalizeVector(lightDirection, lightDirection);

                float dp = dotProduct(lightDirection, normal, vCamera) * 255;
                cout<<dp<<" : ";
                //dp = 1/dp;
                cout<<dp<<endl;

                for (int i = 0; i < 3; i++)
                {
                    multiplyMatrixVector(triTranslated.p[i], triProjected.p[i], matProj);

                    triProjected.p[i].x += 1.0f;
                    triProjected.p[i].y += 1.0f;

                    triProjected.p[i].x *= 0.5f * (float)ScreenWidth();
                    triProjected.p[i].y *= 0.5f * (float)ScreenHeight();
                }

                FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
                             triProjected.p[1].x, triProjected.p[1].y,
                             triProjected.p[2].x, triProjected.p[2].y, {(int)dp, (int)dp, (int)dp});
                DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                             triProjected.p[1].x, triProjected.p[1].y,
                             triProjected.p[2].x, triProjected.p[2].y, {0, 0, 0});
            }
        }

        return true;
    }
};

int main()
{
    Example demo;
    if (demo.Construct(512, 480, 1, 1))
    {
        demo.Start();
    }
    return 0;
}
