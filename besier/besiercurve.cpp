
#include "besiercurve.hpp"


bool besierControll::mouseCheck(int32_t mouseX, int32_t mouseY, bool mHeld, bool mPressed)
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
