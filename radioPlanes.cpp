#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



struct vec3d
{
    float x, y, z;
};  

struct triangle
{
    vec3d p[3];
};

struct mesh
{
    std::vector<triangle> tris;
};


class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Radioplanes";
    }
private:
    mesh meshCube;


public:
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
    // Called once at the start, so create things here
    return true;
}

bool OnUserUpdate(float fElapsedTime) override
{
    // called once per frame
    /*
    for (int x = 0; x < ScreenWidth(); x++)
    {
        for (int y = 0; y < ScreenHeight(); y++)
        
{            Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
        }
    }*/
    FillRect(0,0,ScreenWidth(),ScreenHeight(),olc::BLACK);

    for (auto tri : meshCube.tris)
    return true;
    }
};


int main()
{
    Example demo;
    if (demo.Construct(256, 240, 4, 4))
    {
        demo.Start();
    }

    return 0;
}
