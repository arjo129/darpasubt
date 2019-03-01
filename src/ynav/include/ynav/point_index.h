#ifndef YNAV_OBSTACLE_INDEX_H
#define YNAV_OBSTACLE_INDEX_H

#include <vector>
#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include "ynav/vector.h"

namespace YNav
{

class PointIndex
{
public:
  PointIndex(const std::vector<Vector>& points);
  PointIndex() : PointIndex(std::vector<Vector>()) { }

  bool hasCollision(const Vector& top_left, const Vector& top_right, const Vector& bottom_right, const Vector& bottom_left) const;
  bool getNearest(const Vector& point, int& index, double& distance) const;

private:
  pcl::PointCloud<pcl::PointXY>::Ptr point_cloud_;
  pcl::KdTreeFLANN<pcl::PointXY> point_index_;
};

}

#endif
