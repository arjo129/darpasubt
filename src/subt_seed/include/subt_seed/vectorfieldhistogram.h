#ifndef _subt_seed_vfh_h_
#define _subt_seed_vfh_h_

#include "mapper.h"
#include <vector>
#include <geometry_msgs/Twist.h>

class VectorFieldHist {
    private:
        std::vector polarHist;
    public:
        VectorFieldHist();
        geometry_msgs::Twist sampleAction();
        void fromOccupancyGrid(nav_msgs::OccupancyGrid& grid);
        void visuallizeMap(nav_msgs::OccupancyGrid& grid);
};

#endif