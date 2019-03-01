#ifndef YNAV_ASTAR_NODE_H
#define YNAV_ASTAR_NODE_H

#include <cstddef>
#include "ynav/point.h"
#include "ynav/astar/coordinates.h"

namespace YNav { namespace AStar
{

class Node
{
public:
  Coordinates coordinates;
  Point point;
  Node* parent;
  double g;
  double f;
  bool open;

  Node(const Coordinates& coordinates = Coordinates(), const Point& point = Point());
};

}}

#endif
