#ifndef YNAV_POINT_H
#define YNAV_POINT_H

#include <cmath>
#include "ynav/vector.h"

namespace YNav
{

class Point
{
public:
  Vector position;
  Vector orientation;

  Point(const Vector& position, const Vector& orientation) : position(position), orientation(orientation) { }
  Point() : position(0, 0), orientation(Vector::fromAngle(0)) { }
};

}

#endif
