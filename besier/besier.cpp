#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>
// Override base class with your custom functionality

struct point2d
{
	int32_t x, y;
	point2d() : x(0), y(0) {}
	point2d(int32_t _x, int32_t _y) : x(_x), y(_y) {}

	float dist(const point2d &rhs)
	{	
		return sqrtf(pow(rhs.x - this->x, 2) + pow(rhs.y - this->y, 2));
	}
};

struct besierControll
{
	int nDefined = 0;
	int nMax = 4;
	point2d P[4];
	int nPressed = 100;

	bool mouseCheck(int32_t mouseX, int32_t mouseY, bool mHeld, bool mPressed)
	{
		float this_dist, dist = 10000;
		if (this->nDefined < this->nMax){
			if (mPressed){ 
				P[this->nDefined] = point2d(mouseX, mouseY);
				this->nDefined++;
			} 
		} else {
			if (nPressed > nMax) {
				for (int n = 0; n < this->nDefined; n++){
					this_dist = P[n].dist(point2d(mouseX, mouseY));
					if (this_dist < dist && this_dist < 10) {
						dist = this_dist;
						nPressed = n;
					}
				}
			}
		}
		if (mHeld && nPressed <= nMax) {
			P[nPressed] = point2d(mouseX, mouseY);
		} else {
			nPressed = 100;
		}
		return true;
	};
};

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

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		drawer();
		return true;
	}
};
int main()
{
	Example demo;
	if (demo.Construct(256*4, 240*4, 2, 2))
		demo.Start();
	return 0;
}
point2d Example::lerp(point2d P0, point2d P1, float t) {
	float t_x = (1-t)*P0.x + t*P1.x;
	float t_y = (1-t)*P0.y + t*P1.y;
	return point2d(t_x, t_y);
}
point2d Example::calculateBesier(float t){
	point2d t_A = lerp(B.P[0], B.P[1], t);
	point2d t_B = lerp(B.P[1], B.P[2], t);
	point2d t_C = lerp(B.P[2], B.P[3], t);
	point2d t_D = lerp(t_A, t_B, t);
	point2d t_E = lerp(t_B, t_C, t);
	return lerp(t_D, t_E, t);
}

void Example::drawBesier(void){
	point2d T1, T2;
	if (B.nDefined > 1 && B.nDefined < 4){
	DrawLine(B.P[0].x, B.P[0].y, B.P[1].x, B.P[1].y);
	} else if (B.nDefined == 4){
		DrawLine(B.P[0].x, B.P[0].y, B.P[1].x, B.P[1].y);
		DrawLine(B.P[2].x, B.P[2].y, B.P[3].x, B.P[3].y);
		float t = 0;
		T1 = calculateBesier(t);
		while (t < 1){
			t += 0.01;
			T2 = calculateBesier(t);
			DrawLine(T1.x, T1.y, T2.x, T2.y);
			T1 = T2;
		}
	}
	for (int n = 0; n < B.nDefined; n++){
		FillCircle(B.P[n].x, B.P[n].y, 10);
	}
}
void Example::drawer(void){
	FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));
	mouseX = GetMouseX();
	mouseY = GetMouseY();
	mHeld = GetMouse(0).bHeld;
	mPressed=  GetMouse(0).bPressed;
	B.mouseCheck(mouseX, mouseY, mHeld, mPressed);
	drawBesier();
};
