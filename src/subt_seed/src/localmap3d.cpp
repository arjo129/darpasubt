#include <subt_seed/mapper.h>

using namespace std;

LocalMap3d::LocalMap3d(std::string referenceFrame) {
  frame = referenceFrame;
  tree = new octomap::OcTree(0.1);
}

void LocalMap3d::print_query_info(octomap::point3d query, octomap::OcTreeNode* node) {
  if (node != NULL) {
    cout << "occupancy probability at " << query << ":\t " << node->getOccupancy() << endl;
  }
  else 
    cout << "occupancy probability at " << query << ":\t is unknown" << endl;    
}

void LocalMap3d::insert(const sensor_msgs::PointCloud2ConstPtr& cloud_msg, octomap::point3d& sensor_origin) {
  // Insert complete beam
  // Lazy update of inner nodes after insertion
  // Discretize into octree key cells
  octomap::Pointcloud octo_cloud;
  octomap::pointCloud2ToOctomap(*cloud_msg, octo_cloud);
  this->tree->insertPointCloud(octo_cloud, sensor_origin, -1, true, true);
}

void LocalMap3d::update() {
  this->tree->updateInnerOccupancy();
}
