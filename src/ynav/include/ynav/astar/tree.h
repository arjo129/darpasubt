#ifndef YNAV_ASTAR_TREE_H
#define YNAV_ASTAR_TREE_H

#include <cstddef>
#include <list>
#include <unordered_map>
#include <boost/heap/binomial_heap.hpp>
#include "ynav/vector.h"
#include "ynav/point.h"
#include "ynav/point_index.h"
#include "ynav/astar/coordinates.h"
#include "ynav/astar/node.h"
#include "ynav/astar/heuristic.h"

namespace YNav { namespace AStar
{

class Tree
{
public:
  static constexpr int EXPANDED = 1;
  static constexpr int GOAL_REACHED = 2;
  static constexpr int FULLY_EXPANDED = 3;

  std::list<Node> nodes;
  
  Tree(const Point& source, 
      const Vector& goal, 
      double grid_size, 
      int orientation_steps, 
      double min_steer_radius, 
      int orientation_samples, 
      const Heuristic* heuristic = NULL,
      Vector boundary_top_left = Vector(0, 0), 
      Vector boundary_bottom_right = Vector(0, 0), 
      const std::vector<const PointIndex*>& obstacle_indexes = std::vector<const PointIndex*>());
  Tree(double grid_size, int orientation_steps, double min_steer_radius, int orientation_samples);

  const Point& source() const;
  double grid_size() const;

  int expand(Point& end_point, Node*& end_parent_node);
  int expand();

private:
  Point source_;
  Vector goal_;
  double grid_size_;
  int orientation_steps_;
  double forward_chord_length_;
  double min_steer_radius_;
  int orientation_samples_;
  bool has_goal_;
  const Heuristic* heuristic_;
  Vector boundary_top_left_;
  Vector boundary_bottom_right_;
  std::vector<const PointIndex*> obstacle_indexes_;
 
  // Coordinates -> Node* map.
  struct CoordinatesHash
  {
    std::size_t operator()(const Coordinates& coordinates) const;
  };
  struct CoordinatesEquals  
  {
    bool operator()(const Coordinates& a, const Coordinates& b) const;
  };
  std::unordered_map<Coordinates, Node*, CoordinatesHash, CoordinatesEquals> coordinates_nodes_;
  
  // Open nodes heap.
  struct OpenNodeCompare
  {
    bool operator()(const Node* node1, const Node* node2) const;
  };
  boost::heap::binomial_heap<Node*, boost::heap::compare<OpenNodeCompare>> open_nodes_;

  std::unordered_map<Node*, boost::heap::binomial_heap<Node*, boost::heap::compare<OpenNodeCompare>>::handle_type> open_node_handles_;

  Coordinates getCoordinates(const Point& point) const;
  std::vector<std::pair<Point, double>> sampleForwardPoints(const Point& point) const;
  bool hasCollision(const Point& point) const;
};

}}

#endif
