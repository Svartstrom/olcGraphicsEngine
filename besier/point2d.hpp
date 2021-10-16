#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <cstdint>
#include <cmath>
struct point2d
{
	int32_t x, y;
	point2d() : x(0), y(0) {}
	point2d(int32_t _x, int32_t _y) : x(_x), y(_y) {}

  float dist(const point2d &rhs);
};
#endif
