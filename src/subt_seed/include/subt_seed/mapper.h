#ifndef _subt_seed_mapper_h_
#define  _subt_seed_mapper_h_

#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h> 
#include <pcl/point_types.h>
#include <nav_msgs/OccupancyGrid.h>
#include <unordered_map>
#include <limits>
#include <string>
#include <octomap/octomap.h>
#include <octomap_ros/conversions.h>
#include <octomap/OcTree.h>

class MapCell {
public:
    pcl::PointCloud<pcl::PointXYZI> points;
    int x1, y1;
    bool isOccupiedNaiveMethod();
    //float getSlope();
};

/**
 * This class performs a simple map
 */ 
class LocalMap {
private:
    std::unordered_map<int, std::unordered_map<int, MapCell>> grid;
    int lower_x, lower_y, higher_x, higher_y;
    std::string frame;
    void resetMap();
public:    
    LocalMap(std::string referenceFrame);
    void update(pcl::PointCloud<pcl::PointXYZI>& pcl);
    void toOccupancyGrid(nav_msgs::OccupancyGrid& occupancy);
};

class LocalMap3d {
private:
    std::string frame;
    octomap::OcTree *tree;
public:    
    LocalMap3d(std::string referenceFrame);
    void print_query_info(octomap::point3d query, octomap::OcTreeNode* node);
    void insert(const sensor_msgs::PointCloud2ConstPtr& cloud_msg, octomap::point3d& sensor_origin);
    void update();
};

#endif
