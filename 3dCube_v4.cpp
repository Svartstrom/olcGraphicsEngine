
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
//#include "3dCube_structures.h"
#include <fstream>
#include <strstream>
#include <algorithm>
#include <cmath>

using namespace std;
struct vec3d;
struct triangle;
struct mesh;
struct mat4x4;


struct vec3d
{
    float x, y, z, w;
    vec3d() : x(0), y(0), z(0) {}
    vec3d(int _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x) : x(_x), y(_x), z(_x), w(1) {}
    vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1) {}
    vec3d(int _x, int _y, int _z) : x((float)_x), y((float)_y), z((float)_z), w(1) {}

    vec3d operator + (const vec3d &rhs);
    vec3d operator - (const vec3d &rhs);
    vec3d operator * (const mat4x4 &rhs);
    vec3d operator / (const float  rhs);

    vec3d cross(const vec3d &rhs);
    vec3d normal(void);
    float dot(const vec3d &rhs);

    friend ostream& operator<<(ostream& os, const vec3d& dt);

};

ostream& operator<<(ostream& os, const vec3d& dt)
{
    os << dt.x << " "<< dt.y << " "<< dt.z;
    return os;
}

struct triangle
{
    vec3d p[3] = {0};
    int dp = 0;
    triangle()
    {
        p[0] = vec3d();
        p[1] = vec3d();
        p[2] = vec3d();
        int dp = 0;
    }
    triangle(vec3d _x, vec3d _y, vec3d _z)
    {
        //cout<<"with 3"<<endl;
        p[0] = vec3d(_x);
        p[1] = vec3d(_y);
        p[2] = vec3d(_z);
        int dp = 0;
    }
    triangle(float _x0, float _y0, float _z0, 
             float _x1, float _y1, float _z1, 
             float _x2, float _y2, float _z2)
    {
        p[0] = vec3d(_x0, _y0, _z0);
        p[1] = vec3d(_x1, _y1, _z1);
        p[2] = vec3d(_x2, _y2, _z2);
        int dp = 0;
    }
    friend ostream& operator<<(ostream& os, const triangle& dt);
};
ostream& operator<<(ostream& os, const triangle& dt)
{
    for (int i = 0; i<2; i++){
        os << dt.p[i];
    }
    return os;
}

struct mesh
{
    vector<triangle> tris;
};

struct mat4x4
{
    float m[4][4] = {0};
    friend ostream& operator<<(ostream& os, const mat4x4& dt);
};

ostream& operator<<(ostream& os, const mat4x4& dt)
{
    for(int i = 0;i<4;i++)
    {
        for(int j =0;j<4;j++)
        {
            os << dt.m[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

vec3d vec3d::operator + (const vec3d &rhs) { return vec3d(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);}
vec3d vec3d::operator - (const vec3d &rhs) { return vec3d(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);}
vec3d vec3d::operator / (const float rhs){ return vec3d(this->x / rhs, this->y / rhs, this->z / rhs);}
vec3d vec3d::operator * (const mat4x4 &rhs) 
{
    vec3d out;

    out.x = this->x * rhs.m[0][0] + this->y * rhs.m[1][0] + this->z * rhs.m[2][0] + this->w * rhs.m[3][0];
    out.y = this->x * rhs.m[0][1] + this->y * rhs.m[1][1] + this->z * rhs.m[2][1] + this->w * rhs.m[3][1];
    out.z = this->x * rhs.m[0][2] + this->y * rhs.m[1][2] + this->z * rhs.m[2][2] + this->w * rhs.m[3][2];
    out.w = this->x * rhs.m[0][3] + this->y * rhs.m[1][3] + this->z * rhs.m[2][3] + this->w * rhs.m[3][3];

    return out;
}

vec3d vec3d::cross(const vec3d &rhs)
{
    return vec3d(this->y * rhs.z - this->z * rhs.y,
                 this->z * rhs.x - this->x * rhs.z,
                 this->x * rhs.y - this->y * rhs.x);
}

vec3d vec3d::normal(void)
{
    float l = sqrtf(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    return vec3d(this->x / l, this->y / l, this->z / l);
}

float vec3d::dot(const vec3d &rhs)
{
    return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}

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
    vec3d translator = {0, 0, 5};
    mat4x4 matRotZ, matRotY, matRotX;
    triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

    float fTheta = 0;
    float fNear = 0.1f;
    float fFar = 1.0f;
    float fFovDeg = 90.0f;
    float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
    float fFovRad = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
    float bias = 0.5f;

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
    vector<triangle> cubeMaker(vector<triangle> world, vec3d Origo)
    {
        // triangle(float _x0, float _y0, float _z0, float _x1, float _y1, float _z1, float _x2, float _y2, float _z2)
        //SOUTH
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z));
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z));

        //EAST
        world.push_back(triangle(0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z));
        world.push_back(triangle(0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, 0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z));

        //NORTH
        world.push_back(triangle(0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z));
        world.push_back(triangle(0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z));

        //WEST
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z));
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, -0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z));

        //TOP
        world.push_back(triangle(-0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, -0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z));
        world.push_back(triangle(-0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, 0.5+Origo.z, 0.5+Origo.x, 0.5+Origo.y, -0.5+Origo.z));

        //BOTTOM
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, -0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z));
        world.push_back(triangle(-0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z, 0.5+Origo.x, -0.5+Origo.y, -0.5+Origo.z, 0.5+Origo.x, -0.5+Origo.y, 0.5+Origo.z));

        return world;
    }

public:
    bool OnUserCreate() override
    {
        //meshCube.tris = cubeMaker(meshCube.tris, vec3d());

        meshCube.tris = cubeMaker(meshCube.tris, vec3d(1,0,0));
        meshCube.tris = cubeMaker(meshCube.tris, vec3d(-1,0,0));

        meshCube.tris = cubeMaker(meshCube.tris, vec3d(0,1,0));
        meshCube.tris = cubeMaker(meshCube.tris, vec3d(0,-1,0));

        meshCube.tris = cubeMaker(meshCube.tris, vec3d(0,0,1));
        meshCube.tris = cubeMaker(meshCube.tris, vec3d(0,0,-1));

        matProj = matrixMakeProjection(fFovDeg, fAspectRatio, fFar, fNear);

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

        vector<triangle> vecTriToRaster;

        for (auto tri : meshCube.tris)
        {
            for (int i = 0; i < 3; i++)
            {
                triRotatedZ.p[i] = tri.p[i] * matRotZ;
                triRotatedZX.p[i] = triRotatedZ.p[i] * matRotX;

                triTranslated.p[i] = triRotatedZX.p[i] + translator;
            }

            vec3d normal, line1, line2;

            line1 = triTranslated.p[1] - triTranslated.p[0];
            line2 = triTranslated.p[2] - triTranslated.p[0];

            normal = line1.cross(line2).normal();

            triTranslated.p[0] = triTranslated.p[0] - vCamera;
            float D = triTranslated.p[0].dot(normal);

            if (D < 0.0)
            {
                vec3d lightDirection = {0.0f, 0.0f, -1.0f};
                lightDirection = lightDirection.normal();

                lightDirection = lightDirection - vCamera;
                float dp = lightDirection.dot(normal) * 255;

                for (int i = 0; i < 3; i++)
                {
                    triProjected.p[i] = triTranslated.p[i] * matProj;
                    if (triProjected.p[i].w != 0)
                    {
                        triProjected.p[i] = triProjected.p[i] / triProjected.p[i].w;
                    }
                    triProjected.p[i] = triProjected.p[i] + vec3d(1.0f, 1.0f, 0.0f);
                    triProjected.dp = dp;

                    triProjected.p[i].x *= 0.5f * (float)ScreenWidth();
                    triProjected.p[i].y *= 0.5f * (float)ScreenHeight();
                }

                vecTriToRaster.push_back(triProjected);
/*
                FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
                             triProjected.p[1].x, triProjected.p[1].y,
                             triProjected.p[2].x, triProjected.p[2].y, {(int)dp, (int)dp, (int)dp});
                DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                             triProjected.p[1].x, triProjected.p[1].y,
                             triProjected.p[2].x, triProjected.p[2].y, {0, 0, 0});*/
            }
        }

        sort(vecTriToRaster.begin(), vecTriToRaster.end(), [](triangle &t1,triangle &t2)
        {
            float z1 = (t1.p[0].z+t1.p[1].z+t1.p[2].z) / 3.0f;
            float z2 = (t2.p[0].z+t2.p[1].z+t2.p[2].z) / 3.0f;
            return z1 > z2;
        } );

        for (auto &triProjected : vecTriToRaster)
        {
            FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
                        triProjected.p[1].x, triProjected.p[1].y,
                        triProjected.p[2].x, triProjected.p[2].y, {triProjected.dp, triProjected.dp, triProjected.dp});//{(int)dp, (int)dp, (int)dp});
            //DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
            //            triProjected.p[1].x, triProjected.p[1].y,
            //            triProjected.p[2].x, triProjected.p[2].y, {0, 0, 0});
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
