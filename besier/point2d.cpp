#include "point2d.hpp"
float point2d::dist(const point2d &rhs)
{
  return sqrtf(pow(rhs.x - this->x, 2) + pow(rhs.y - this->y, 2));
}
