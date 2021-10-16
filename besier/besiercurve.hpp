#include "point2d.hpp"
#include <cstdint>
//#include <cmath>
struct besierControll
{
	int nDefined = 0;
	int nMax = 4;
	point2d P[4];
	int nPressed = 100;

	bool mouseCheck(int32_t mouseX, int32_t mouseY, bool mHeld, bool mPressed);
};
