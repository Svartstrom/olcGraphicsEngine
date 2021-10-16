#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "point2d.hpp"
#include "besiercurve.hpp"
#include <cmath>

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
public:
	int32_t mouseX, mouseY;
	int32_t delimX, delimY;
	bool mHeld, mPressed;
	besierControll B;

	void drawer(void);
	point2d calculateBesier(float t);
	void drawBesier(void);
	point2d lerp(point2d P1, point2d P2, float t);

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};
