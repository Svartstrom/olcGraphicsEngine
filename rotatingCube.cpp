#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
};  

struct LINE
{
    vec3d start,end;
};

struct triangle
{
    vec3d p[3];
    vec3d normal()
    {
        vec3d o, A, B;

        A = {p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z};
        B = {p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z};
        
        o = {A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x};

        float l = 10*sqrtf(o.x*o.x + o.y*o.y + o.z*o.z);
        o.x /= l; 
        o.y /= l; 
        o.z /= l;
        return o;
    }
    void rotateX (float fTheta)
    {
	    matRot.m[0][0] = 1;
	    matRot.m[1][1] = cosf(fTheta);
	    matRot.m[1][2] = sinf(fTheta);
	    matRot.m[2][1] = -sinf(fTheta);
	    matRot.m[2][2] = cosf(fTheta);
	    matRot.m[3][3] = 1;
    }
    void rotatey( float fTheta)
    {
    	matRot.m[0][0] = cosf(fTheta);
	    matRot.m[2][0] = sinf(fTheta);
	    matRot.m[0][2] = -sinf(fTheta);
	    matRot.m[2][2] = cosf(fTheta);
	    matRot.m[1][1] = 1;
	    matRot.m[3][3] = 1;
    }
    void rotateZ( float fTheta)
    {
    	matRot.m[0][0] = cosf(fTheta);
	    matRot.m[0][1] = sinf(fTheta);
	    matRot.m[1][0] = -sinf(fTheta);
	    matRot.m[1][1] = cosf(fTheta);
	    matRot.m[2][2] = 1;
	    matRot.m[3][3] = 1;
    }
    vec3d MultiplyMatrxVector(const vec3d &i, vec3d &o, const mat4x4 &m)
    {
        o.x     = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y     = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z     = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }

    float dotProduct(vec3d &A, vec3d &B)
    {
        return (float)(A.x*B.x + A.y*B.y + A.z*B.z);
    }
};

struct mesh
{
    std::vector<triangle> tris;
};


struct mat4x4
{
    float m[4][4] = { 0.0 };
};

class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "rotatinCube";
    }
private:
    mesh meshCube;
    mesh meshTri;
    mat4x4 matProj;
    float fTheta=0;
    vec3d vCamera;
    LINE normLine;

    void MultiplyMatrxVector(const vec3d &i, vec3d &o, const mat4x4 &m)
    {
        o.x     = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y     = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z     = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }

public:

    float dotProduct(vec3d &A, vec3d &B)
    {
        return (float)(A.x*B.x + A.y*B.y + A.z*B.z);
    }
    bool OnUserCreate() override
    {
    	
        meshCube.tris = {

        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST                                                      
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH                                                     
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST                                                      
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP                                                       
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM                                                    
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
        };

		meshTri.tris = {{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f } };
        //meshTri.tris = meshCube.tris.at(1);

        float fNear = 0.1f;
        float fFar  = 1000.0f;
        float fFov  = 90.0f;
        float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
        float fFovRad = 1.0f / tanf(3.141592f * fFov * 0.5 / 180.0f );

        matProj.m[0][0] = fAspectRatio * fFovRad;
        matProj.m[1][1] = fFovRad;
        matProj.m[2][2] = fFar / (fFar - fNear);
        matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;

    	return true;
	}


	//vec3d matVecMult
	bool OnUserUpdate(float fElapsedTime) override
	{
	    // called once per frame

	    FillRect(0,0,ScreenWidth(),ScreenHeight(),olc::BLACK);

	    mat4x4 matRotZ, matRotX, unity;
	    triangle triProjected, triTranslated,triRotatedZ, triRotatedZX;
	    vec3d normal, eyeLine;
	    eyeLine.z = -1;
	    vCamera.z = -5;

	    fTheta += 1.0f * fElapsedTime;

	    unity.m[0][0] = 1;
	    unity.m[1][1] = 1;
	    unity.m[2][2] = 1;
	    unity.m[3][3] = 1;

	    matRotZ.m[0][0] = cosf(fTheta);
	    matRotZ.m[0][1] = sinf(fTheta);
	    matRotZ.m[1][0] = -sinf(fTheta);
	    matRotZ.m[1][1] = cosf(fTheta);
	    matRotZ.m[2][2] = 1;
	    matRotZ.m[3][3] = 1;

	    matRotX.m[0][0] = 1;
	    matRotX.m[1][1] = cosf(fTheta);
	    matRotX.m[1][2] = sinf(fTheta);
	    matRotX.m[2][1] = -sinf(fTheta);
	    matRotX.m[2][2] = cosf(fTheta);
	    matRotX.m[3][3] = 1;

	    for (auto tri : meshTri.tris)
	    {
	        for (int i = 0; i < 3; i++)
	        {
	            MultiplyMatrxVector(tri.p[i], triRotatedZ.p[i],matRotZ);
	            MultiplyMatrxVector(triRotatedZ.p[i], triRotatedZX.p[i],matRotX);

	            triTranslated = triRotatedZX;
	            triTranslated.p[i].z = triRotatedZX.p[i].z + 3.0f;
	            triTranslated.p[i].x = triRotatedZX.p[i].x;

	            MultiplyMatrxVector(triTranslated.p[i], triProjected.p[i], matProj);
	            triProjected.p[i].x += 1.0f; 
	            triProjected.p[i].y += 1.0f;
	            triProjected.p[i].x *= (0.5f * (float)ScreenWidth()); 
	            triProjected.p[i].y *= (0.5f * (float)ScreenHeight());
	        }
	        
	        normLine.start = triTranslated.p[0];
	        normLine.end = triTranslated.normal();

	        MultiplyMatrxVector(normLine.start, normLine.start, matProj);
	        normLine.start.x += 1.0f; 
	        normLine.start.y += 1.0f;
	        normLine.start.z += 3.0f;
	        normLine.start.x *= (0.5f * (float)ScreenWidth()); 
	        normLine.start.y *= (0.5f * (float)ScreenHeight());
	        MultiplyMatrxVector(normLine.end, normLine.end, matProj);
	        normLine.end.x += 1.0f; 
	        normLine.end.y += 1.0f;
	        normLine.end.z += 3.0f;
	        normLine.end.x *= (0.5f * (float)ScreenWidth()); 
	        normLine.end.y *= (0.5f * (float)ScreenHeight());

	        normal = triTranslated.normal();
	        

	        eyeLine.x = triTranslated.p[0].x - vCamera.x;
	        eyeLine.y = triTranslated.p[0].y - vCamera.y;
	        eyeLine.z = triTranslated.p[0].z - vCamera.z;
	        eyeLine.x = 0;
            eyeLine.y = 0;
            eyeLine.z = -1;
	        
	        if (dotProduct(normal,eyeLine) > 0.0f)
	        {
	            DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, 
	                         triProjected.p[1].x, triProjected.p[1].y, 
	                         triProjected.p[2].x, triProjected.p[2].y, olc::WHITE);
	        }else
	        {
	            DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, 
	                         triProjected.p[1].x, triProjected.p[1].y, 
	                         triProjected.p[2].x, triProjected.p[2].y, olc::GREEN);
	        }
	        DrawLine(normLine.start.x, normLine.start.y, normLine.end.x, normLine.end.y);

	    }

	    return true;
    }
};


int main()
{
    Example demo;
    if (demo.Construct(256*2, 240*2, 1, 1))
    {
        demo.Start();
    }

    return 0;
}
