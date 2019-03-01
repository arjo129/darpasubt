#ifndef YNAV_ASTAR_COORDINATES_H
#define YNAV_ASTAR_COORDINATES_H

#include <ros/ros.h>
#include <functional>
#include "ynav/point.h"

namespace YNav { namespace AStar
{

class Coordinates
{
public:
  int x;
  int y;
  int o;

  Coordinates(int x = 0, int y = 0, int o = 0);
};

}}

#endif
