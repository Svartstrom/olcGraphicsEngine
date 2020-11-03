
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "3d_vec3d.hpp"
#include "3d_mesh.hpp"
#include "3d_mat4x4.hpp"
#include "3d_triangle.hpp"
#include "3d_camera.hpp"
#include "3d_utilities.hpp"

#include <fstream>
#include <strstream>
#include <algorithm>
#include <cmath>

using namespace std;

class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Lines";
    }

private:
    vector<mesh> world;
    mesh meshCube;
    mat4x4 matProj;
    vec3d vUp;
    camera Camera = camera(vec3d(5,5,5),vec3d(0,0,1));
    vec3d vLookDir, vTarget;
    
    vec3d translator;
    vec3d normal, line1, line2;
    mat4x4 matRotZ, matRotY, matRotX, matWorld, matTrans;
    triangle triProjected, triTranslated, triRotatedZ, triRotatedZX, triViewed;
    
    float fYaw = 0;
    float fTheta = 0;
    float fNear = 0.1f;
    float fFar = 1.0f;
    float fFovDeg = 90.0f;
    float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
    float fFovRad = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.141592f);
    float bias = 0.5f;

    vec3d Vector_IntersectPlane(vec3d & plane_p, vec3d &plane_n, vec3d &line_start, vec3d &line_end)
    {
        plane_n = plane_n.normal();
        float plane_d = -plane_n.dot(plane_p);
        float ad = line_start.dot(plane_n);
        float bd = line_end.dot(plane_n);
        float t  = (-plane_d -ad) / (bd - ad);
        vec3d lineStartToEnd = line_end - line_start;
        vec3d lineToIntersect = lineStartToEnd * t;
        return line_start + lineToIntersect;
    }

    int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
    {
        vec3d* insidePoints[3];  int nInsidePointCount  = 0;
        vec3d* outsidePoints[3]; int nOutsidePointCount = 0;

        plane_n = plane_n.normal();

        auto dist = [&](vec3d &p)
        {
            vec3d n = p.normal();
            return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dot(plane_p));
        };

        for (int ii = 0; ii < 3; ii++)
        {
            float dP = dist(in_tri.p[ii]);

            if (dP > 0) {insidePoints[nInsidePointCount++] = &in_tri.p[ii];}
            else {outsidePoints[nOutsidePointCount++] = &in_tri.p[ii];}
        }

        if (nInsidePointCount == 0)
        {
            return 0;
        }

        if (nInsidePointCount == 3)
        {
            out_tri1 = in_tri;
            return 1;
        }

        if (nInsidePointCount == 1 && nOutsidePointCount == 2)
        {
            out_tri1.col = vec3d(255,0,0);//in_tri.col;

            out_tri1.p[0] = *insidePoints[0];
            out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[0]);
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[1]);
            
            return 1;
        }
        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
        {
            out_tri1.col = vec3d(0, 255, 0);//in_tri.col;
            out_tri2.col = vec3d(0, 0, 255);//in_tri.col;

            out_tri1.p[0] = *insidePoints[0];
            out_tri1.p[1] = *insidePoints[1];
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[0]);

            out_tri2.p[0] = *insidePoints[1];
            out_tri2.p[1] = out_tri1.p[2];//Vector_IntersectPlane(plane_p,plane_n,*insidePoints[1],*outsidePoints[0]);
            out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *insidePoints[1], *outsidePoints[0]);
            
            return 2;
        }
    }

    vector<triangle> cubeMaker(vector<triangle> world, vec3d dia, vec3d Origo)
    {
        // triangle(float _x0, float _y0, float _z0, float _x1, float _y1, float _z1, float _x2, float _y2, float _z2)
        //SOUTH
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        //EASTx
        world.push_back(triangle(    0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        world.push_back(triangle(    0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        //NORTHx
        world.push_back(triangle(    0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        world.push_back(triangle(    0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        //WESTx
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        //TOPx
        world.push_back(triangle(   -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        world.push_back(triangle(   -0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x,  0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        //BOTTOMx
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                    -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z));
        world.push_back(triangle(   -0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y, -0.5 * dia.z + Origo.z, 
                                     0.5 * dia.x + Origo.x, -0.5 * dia.y + Origo.y,  0.5 * dia.z + Origo.z));
        return world;
    }

public:
    bool OnUserCreate() override
    {
        meshCube.tris = cubeMaker(meshCube.tris, vec3d(1,1,1), vec3d(0,0,0));
        world.push_back(meshCube);
        translator = {0, 0, 10};

        matProj = matrixMakeProjection(fFovDeg, fAspectRatio, fFar, fNear);
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        vec3d vForward = Camera.dir * 8.0f * fElapsedTime;
        vec3d vLeft = Camera.dir * 8.0f * fElapsedTime * matrixRotationY(3.141592/2.0f);
        fTheta += 1.0f * fElapsedTime;

        if (GetKey(olc::Key::DOWN ).bHeld) { Camera.pos.y += 1.0f * fElapsedTime; }
        if (GetKey(olc::Key::UP   ).bHeld) { Camera.pos.y -= 1.0f * fElapsedTime; }
        
        if (GetKey(olc::Key::RIGHT).bHeld) { Camera.pos = Camera.pos - vLeft; }
        if (GetKey(olc::Key::LEFT ).bHeld) { Camera.pos = Camera.pos + vLeft; }

        if (GetKey(olc::Key::W).bHeld) { Camera.pos = Camera.pos + vForward; }
        if (GetKey(olc::Key::S).bHeld) { Camera.pos = Camera.pos - vForward; }

        if (GetKey(olc::Key::D).bHeld) { fYaw -= 2.0f * fElapsedTime; }
        if (GetKey(olc::Key::A).bHeld) { fYaw += 2.0f * fElapsedTime; }
        
        //matRotX = matrixRotationX(fTheta * bias);
        //matRotY = matrixRotationY(fTheta);
        //matRotZ = matrixRotationZ(fTheta);

        matTrans = matrixTranslation(translator);
        matWorld = matrixUnit();
        matWorld = matWorld * matTrans;

        vUp = vec3d(0, 1, 0);
        vTarget = vec3d(0, 0, 1);

        mat4x4 matCameraRot = matrixRotationY(fYaw);
        Camera.dir = vTarget * matCameraRot;
        vTarget = Camera.pos + Camera.dir;

        mat4x4 matView = PointAt(Camera.pos, vTarget, vUp);

        vector<triangle> vecTriToRaster;

        for (auto meshCube: world)
        {
            for (auto tri : meshCube.tris)
            {
                for (int i = 0; i < 3; i++)
                {
                    triTranslated.p[i] = tri.p[i] * matWorld;
                }

                line1 = triTranslated.p[1] - triTranslated.p[0];
                line2 = triTranslated.p[2] - triTranslated.p[0];

                normal = line1.cross(line2).normal();

                vec3d cameraRay = triTranslated.p[0] - Camera.pos;
                float D = cameraRay.dot(normal);

                if (D < 0.0)
                {
                    vec3d lightDirection = {0.0f, 0.0f, -1.0f};
                    triangle clipped[2];
                    int nClippedTriangles = 0;

                    lightDirection = lightDirection - Camera.pos;
                    lightDirection = lightDirection.normal();
                    //vec3d dp = vec3d(1,1,1)*lightDirection.dot(normal) * 255;
                    float dp = max(lightDirection.dot(normal), 0.1f);
                    triViewed.col = triTranslated.col * dp;

                    for (int i = 0; i < 3; i++)
                    {
                        triViewed.p[i] = triTranslated.p[i] * matView;
                    }
                    
                    nClippedTriangles = Triangle_ClipAgainstPlane(vec3d(0.0f, 0.0f, 0.1f), vec3d(0, 0, 1), triViewed, clipped[0], clipped[1]);

                    for (int nn = 0; nn < nClippedTriangles; nn++)
                    {  
                        for (int i = 0; i < 3; i++)
                        {
                            triProjected.p[i] = clipped[nn].p[i] * matProj;
                            triProjected.col = clipped[nn].col;
                            if (triProjected.p[i].w != 0)
                            {
                                triProjected.p[i] = triProjected.p[i] / triProjected.p[i].w;
                            }
                            triProjected.p[i] = triProjected.p[i] + vec3d(1.0f, 1.0f, 0.0f);
                            //triProjected.col = dp;

                            triProjected.p[i].x *= 0.5f * (float)ScreenWidth();
                            triProjected.p[i].y *= 0.5f * (float)ScreenHeight();
                        }
                        vecTriToRaster.push_back(triProjected);
                    }
                }
            }
        }

        sort(vecTriToRaster.begin(), vecTriToRaster.end(), [](triangle &t1, triangle &t2)
        {
            float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
            float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
            return z1 > z2;
        } );

        // Clear Screen
        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));
        
        float SH = (float)ScreenHeight();
        float SW = (float)ScreenWidth();
        for (auto triToRaster : vecTriToRaster)
        {
            triangle clipped[2];
            list<triangle> listTriangles;
            listTriangles.push_back(triToRaster);
            int nNewTriangles = 1;

            for (int p = 0; p < 4; p++)
            {
                int nTrisToAdd = 0;
                while ( nNewTriangles > 0)
                {
                    triangle test = listTriangles.front();
                    listTriangles.pop_front();
                    nNewTriangles--;

                    switch (p)
                    {
                        case 0: nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f,   0.0f,   0.0f }, { 0.0f,  1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					    case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f,   SH - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					    case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f,   0.0f,   0.0f }, { 1.0f,  0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					    case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ SW - 1, 0.0f,   0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    }
                    for (int w = 0; w < nTrisToAdd; w++)
                    {
                        listTriangles.push_back(clipped[w]);
                    }
                }
                nNewTriangles = listTriangles.size();
            }

            for (auto &triToDraw : listTriangles)
            {
                FillTriangle(triToDraw.p[0].x, triToDraw.p[0].y,
                             triToDraw.p[1].x, triToDraw.p[1].y,
                             triToDraw.p[2].x, triToDraw.p[2].y, {triToDraw.col.x, triToDraw.col.y, triToDraw.col.z});
                DrawTriangle(triToDraw.p[0].x, triToDraw.p[0].y,
                             triToDraw.p[1].x, triToDraw.p[1].y,
                             triToDraw.p[2].x, triToDraw.p[2].y, {0, 0, 0});
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
