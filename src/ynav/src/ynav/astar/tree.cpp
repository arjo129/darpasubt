#include <ros/ros.h>
#include <cmath>
#include "ynav/astar/tree.h"

namespace YNav { namespace AStar
{

std::size_t Tree::CoordinatesHash::operator()(const Coordinates& coordinates) const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, coordinates.x);
    boost::hash_combine(seed, coordinates.y);
    boost::hash_combine(seed, coordinates.o);
    return seed;
}

bool Tree::CoordinatesEquals::operator()(const Coordinates& a, const Coordinates& b) const
{
  return a.x == b.x && a.y == b.y && a.o == b.o;
}
    
bool Tree::OpenNodeCompare::operator()(const Node* node1, const Node* node2) const
{
  return node1->f > node2->f;
}

Tree::Tree(const Point& source, 
    const Vector& goal, 
    double grid_size, 
    int orientation_steps, 
    double min_steer_radius, 
    int orientation_samples, 
    const Heuristic* heuristic,
    Vector boundary_top_left, 
    Vector boundary_bottom_right,
    const std::vector<const PointIndex*>& obstacle_indexes)
    : source_(source), 
    goal_(goal), 
    grid_size_(grid_size), 
    orientation_steps_(orientation_steps), 
    forward_chord_length_(std::sqrt(2) * grid_size_),
    min_steer_radius_(min_steer_radius), 
    orientation_samples_(orientation_samples),
    has_goal_(true),
    heuristic_(heuristic),
    boundary_top_left_(boundary_top_left),
    boundary_bottom_right_(boundary_bottom_right),
    obstacle_indexes_(obstacle_indexes)
{
  nodes.emplace_back(Coordinates(0, 0, 0), source);
  Node* node = &nodes.back();
  coordinates_nodes_[node->coordinates] = node;
  if (has_goal_ && heuristic_ != NULL)
  {
    Vector relative_goal = (goal - source.position).rotate(-source.orientation.angle());
    if (heuristic_->contains(relative_goal))
    {
      node->f = heuristic_->get(relative_goal);
    }
    else
    {
      heuristic_ = NULL;
    }
  }
  open_node_handles_[node] = open_nodes_.push(node);
}

Tree::Tree(double grid_size, int orientation_steps, double min_steer_radius, int orientation_samples)
    : source_(Point()),
    grid_size_(grid_size),
    orientation_steps_(orientation_steps),
    forward_chord_length_(std::sqrt(2) * grid_size_),
    min_steer_radius_(min_steer_radius),
    orientation_samples_(orientation_samples),
    has_goal_(false),
    heuristic_(NULL)
{
  nodes.emplace_back(Coordinates(0, 0, 0), Point());
  Node* node = &nodes.back();
  coordinates_nodes_[node->coordinates] = node;
  open_node_handles_[node] = open_nodes_.push(node);
}

const Point& Tree::source() const
{
  return source_;
}

double Tree::grid_size() const
{
  return grid_size_;
}

Coordinates Tree::getCoordinates(const Point& point) const
{
  Vector relative_position = (point.position - source_.position).rotate(-source_.orientation.angle());
  double x = relative_position.x;
  double y = relative_position.y;
  double o = M_PI + source_.orientation.angleTo(point.orientation);

  return Coordinates(
      (int)round(x / grid_size_), 
      (int)round(y / grid_size_), 
      (int)round(o * orientation_steps_ / (2 * M_PI)) % orientation_steps_);
}

std::vector<std::pair<Point, double>> Tree::sampleForwardPoints(const Point& point) const
{
  double theta_max = std::asin(forward_chord_length_ / (2 * min_steer_radius_));
  double theta_step = theta_max / orientation_samples_;

  std::vector<std::pair<Point, double>> pairs;
  pairs.emplace_back(
      Point(point.position + forward_chord_length_ * point.orientation, point.orientation), 
      forward_chord_length_);

  for (int i = 1; i <= orientation_samples_; i++)
  {
    double theta = theta_step * i;
    double segment_length = forward_chord_length_ * theta / std::sin(theta);
    pairs.emplace_back(
        Point(
            point.position + forward_chord_length_ * point.orientation.rotate(theta), 
            point.orientation.rotate(2 * theta)), 
        segment_length);
    pairs.emplace_back(
        Point(
            point.position + forward_chord_length_ * point.orientation.rotate(-theta), 
            point.orientation.rotate(2 * -theta)), 
        segment_length);
  }

  return pairs;
}

bool Tree::hasCollision(const Point& point) const
{
  Vector x_dir = point.orientation;
  Vector y_dir = point.orientation.rotate(M_PI / 2);
  Vector top_left = point.position + boundary_top_left_.x * x_dir + boundary_top_left_.y * y_dir;
  Vector top_right = point.position + boundary_top_left_.x * x_dir + boundary_bottom_right_.y * y_dir;
  Vector bottom_right = point.position + boundary_bottom_right_.x * x_dir + boundary_bottom_right_.y * y_dir;
  Vector bottom_left = point.position + boundary_bottom_right_.x * x_dir + boundary_top_left_.y * y_dir;

  for (const PointIndex* obstacle_index : obstacle_indexes_)
  {
    if (obstacle_index->hasCollision(top_left, top_right, bottom_right, bottom_left))
    {
      return true;
    }
  }
  return false;
}

int Tree::expand(Point& end_point, Node*& end_parent_node)
{
  if (open_nodes_.empty()) return FULLY_EXPANDED;

  Node* node = open_nodes_.top();
  open_nodes_.pop();

  // Remove from open set.
  node->open = false;

  for (const std::pair<Point, double>& pair : sampleForwardPoints(node->point))
  {
    const Point& forward_point = pair.first;

    if (hasCollision(forward_point)) continue;

    double forward_cost = pair.second;
    double forward_point_heuristic = 0;
    if (has_goal_ && heuristic_ != NULL)
    {
      Vector relative_goal = (goal_ - forward_point.position).rotate(-forward_point.orientation.angle());
      if (!heuristic_->contains(relative_goal)) continue;
      forward_point_heuristic = heuristic_->get(relative_goal);
    }

    if (has_goal_ && (goal_ - forward_point.position).norm() < forward_chord_length_)
    {
      end_point = forward_point;
      end_parent_node = node;
      return GOAL_REACHED;
    }

    Coordinates forward_coordinates = getCoordinates(forward_point);
    auto it_forward_node = coordinates_nodes_.find(forward_coordinates);
    
    if (it_forward_node == coordinates_nodes_.end())
    {
      nodes.emplace_back(forward_coordinates, forward_point);
      Node* forward_node = &nodes.back();
      forward_node->parent = node;
      forward_node->g = node->g + forward_cost;
      forward_node->f = forward_node->g + forward_point_heuristic;
      coordinates_nodes_[forward_coordinates] = forward_node;
      open_node_handles_[forward_node] = open_nodes_.push(forward_node);
    }
    else 
    {
      Node* forward_node = it_forward_node->second; 
      if (forward_node->open)
      {
        if (node->g + forward_cost < forward_node->g)
        {
          forward_node->point = forward_point;
          forward_node->parent = node;
          forward_node->g = node->g + forward_cost;
          forward_node->f = forward_node->g + forward_point_heuristic;
          open_nodes_.update(open_node_handles_[forward_node]);
        }
      }
    }
  }

  return EXPANDED;
}

int Tree::expand()
{
  Point end_point;
  Node* end_parent_node;
  return expand(end_point, end_parent_node);
}

}}
