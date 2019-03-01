#include <fstream>
#include <cmath>
#include <boost/functional/hash.hpp>
#include "ynav/astar/heuristic.h"

namespace YNav { namespace AStar
{

size_t Heuristic::XYPairHash::operator()(const std::pair<int, int> p) const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, p.first);
  boost::hash_combine(seed, p.second);
  return seed;
}

bool Heuristic::XYPairEquals::operator()(const std::pair<int, int>& l, const std::pair<int, int>& r) const
{
  return l.first == r.first && l.second == r.second;
}

Heuristic::Heuristic(double grid_size) 
    : grid_size_(grid_size)
{

}

Heuristic Heuristic::loadFromFile(std::string file_name)
{
  std::ifstream file(file_name, std::ios::in | std::ios::binary);
  double grid_size = 0;
  int count = 0;
  file.read(reinterpret_cast<char*>(&grid_size), sizeof(double));
  Heuristic heuristic(grid_size);
  file.read(reinterpret_cast<char*>(&count), sizeof(int));
  while (!file.eof())
  {
    int x;
    int y;
    double cost;
    file.read(reinterpret_cast<char*>(&x), sizeof(int));
    file.read(reinterpret_cast<char*>(&y), sizeof(int));
    file.read(reinterpret_cast<char*>(&cost), sizeof(double));
    heuristic.costs[std::pair<int, int>(x, y)] = cost;
  }
  file.close();
  return heuristic;
}

void Heuristic::saveToFile(std::string file_name) const
{
  std::ofstream file(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
  file.write(reinterpret_cast<const char*>(&grid_size_), sizeof(double));
  int count = costs.size();
  file.write(reinterpret_cast<char*>(&count), sizeof(int));
  for (const auto& p : costs)
  {
    int x = p.first.first;
    int y = p.first.second;
    double cost = p.second;
    file.write(reinterpret_cast<char*>(&x), sizeof(int));
    file.write(reinterpret_cast<char*>(&y), sizeof(int));
    file.write(reinterpret_cast<char*>(&cost), sizeof(double));
  }
  file.close();
}

double Heuristic::grid_size() const
{
  return grid_size_;
}

std::pair<int, int> Heuristic::getCoordinates(const Vector& position) const
{
  return std::pair<int, int>(
      (int)std::round(position.x / grid_size_),
      (int)std::round(position.y / grid_size_));
}

bool Heuristic::contains(const Vector& position) const 
{ 
  return costs.find(getCoordinates(position)) != costs.end(); 
}
  
double Heuristic::get(const Vector& position) const 
{ 
  return costs.at(getCoordinates(position)); 
}
  
void Heuristic::update(const Vector& position, double value)
{
  std::pair<int, int> coordinates = getCoordinates(position);
  auto it_cost = costs.find(coordinates);
  if (it_cost == costs.end())
  {
    costs[coordinates] = value;
  }
  else
  {
    it_cost->second = std::min(value, it_cost->second);
  }
}

}}
