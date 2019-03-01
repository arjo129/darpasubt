#include <ros/ros.h>
#include <cmath>
#include "ynav/astar/tree.h"

namespace YNav { namespace AStar
{

Node::Node(const Coordinates& coordinates, const Point& point)
    : coordinates(coordinates), point(point), parent(NULL), g(0), f(0), open(true)
{

}

}}
