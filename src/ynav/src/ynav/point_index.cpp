#include <cmath>
#include "ynav/point_index.h"

namespace YNav
{

PointIndex::PointIndex(const std::vector<Vector>& points)
    : point_cloud_(new pcl::PointCloud<pcl::PointXY>())
{
  point_cloud_->width = points.size();
  point_cloud_->height = 1;
  point_cloud_->points.resize(points.size());
  for (int i = 0; i < points.size(); i++)
  {
    point_cloud_->points[i].x = points[i].x;
    point_cloud_->points[i].y = points[i].y;
  }

  point_index_.setEpsilon(0);
  if (!points.empty()) point_index_.setInputCloud(point_cloud_);
}

bool PointIndex::hasCollision(const Vector& top_left, const Vector& top_right, const Vector& bottom_right, const Vector& bottom_left) const
{
  if (point_cloud_->empty()) return false;

  Vector query_center = (top_left + bottom_right) / 2;
  double query_radius = (top_left - bottom_right).norm() / 2;
  pcl::PointXY pcl_query_center; 
  pcl_query_center.x = query_center.x; 
  pcl_query_center.y = query_center.y;
  
  std::vector<int> indexes;
  std::vector<float> distances;
  if (!point_index_.radiusSearch(pcl_query_center, query_radius, indexes, distances)) return false;

  for (int i : indexes)
  {
    Vector point(point_index_.getInputCloud()->points[i].x, 
        point_index_.getInputCloud()->points[i].y);
    
    double a = (point - top_left).dotProduct(bottom_left - top_left);
    if (a < 0) continue;
    if (a > (bottom_left - top_left).dotProduct(bottom_left - top_left)) continue;

    a = (point - top_left).dotProduct(top_right - top_left);
    if (a < 0) continue;
    if (a > (top_right - top_left).dotProduct(top_right - top_left)) continue;

    return true;
  }

  return false;
}

bool PointIndex::getNearest(const Vector& point, int& index, double& distance) const
{
  if (point_cloud_->empty()) return false;

  pcl::PointXY pcl_point;
  pcl_point.x = point.x;
  pcl_point.y = point.y;
  
  std::vector<int> indexes(1);
  std::vector<float> distances(1);
  point_index_.nearestKSearch(pcl_point, 1, indexes, distances);
  
  index = indexes[0];
  distance = std::sqrt(distances[0]);

  return true;
}

}
