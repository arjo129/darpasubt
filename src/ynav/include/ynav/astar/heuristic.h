#ifndef YNAV_ASTAR_HEURISTIC_H
#define YNAV_ASTAR_HEURISTIC_H

#include <unordered_map>
#include "ynav/vector.h"

namespace YNav { namespace AStar
{

class Heuristic
{
public:
  struct XYPairHash
  {
    size_t operator()(const std::pair<int, int> p) const;
  };
  struct XYPairEquals
  {
    bool operator()(const std::pair<int, int>& l, const std::pair<int, int>& r) const;
  };
  std::unordered_map<std::pair<int, int>, double, XYPairHash, XYPairEquals> costs;

  Heuristic(double grid_size = 0);
  static Heuristic loadFromFile(std::string file_name);

  void saveToFile(std::string file_name) const;
  double grid_size() const;
  bool contains(const Vector& position) const;
  double get(const Vector& position) const;
  void update(const Vector& position, double value);

private:
  double grid_size_;
  
  std::pair<int, int> getCoordinates(const Vector& position) const;
};

}}

#endif
