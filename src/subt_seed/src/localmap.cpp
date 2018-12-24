#include <subt_seed/mapper.h>

bool MapCell::isOccupiedNaiveMethod(){
    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i].z < 1 && points[i].z > 0) return true;
    }
    return false;
}

LocalMap::LocalMap(std::string referenceFrame) {
    frame = referenceFrame;
}

void LocalMap::resetMap(){

    //Set
    lower_x = std::numeric_limits<int>::max();
    lower_y = std::numeric_limits<int>::max();
    higher_x = std::numeric_limits<int>::min();
    higher_y = std::numeric_limits<int>::min();

    //Erase the previous map
    for(auto i : grid){
        grid[i.first].clear();
    }
    grid.clear();

}

void LocalMap::update(pcl::PointCloud<pcl::PointXYZI>& cloud){
    
    //Reset the map once drawn
    resetMap();

    //Plot point cloud on occupancy grid
    for(unsigned int i  = 0; i < cloud.size(); i++){
        pcl::PointXYZI pt = cloud.at(i);
        int x = (int)round(pt.x);
        int y = (int)round(pt.y);
        if(grid.find(x) != grid.end()){
            if(grid[x].find(y) != grid[x].end()) {
                MapCell mapCell;
                mapCell.points.push_back(pt);
                mapCell.x1 = x;
                mapCell.y1 = y;
                grid[x][y] = mapCell;                
            } else {
                grid[x][y].points.push_back(pt);
            }
        } else {
            MapCell mapCell;
            mapCell.points.push_back(pt);
            mapCell.x1 = x;
            mapCell.y1 = y;
            grid[x][y] = mapCell; 
        }
        if(x < lower_x) lower_x = x;
        if(y < lower_y) lower_y = y;
        if(x > higher_x) higher_x = x;
        if(y > higher_y) higher_y = y;
    }
}


void LocalMap::toOccupancyGrid(nav_msgs::OccupancyGrid& occupancyGrid) {
    
    for(int y = lower_y; y < higher_y; y++) {
        for(int x = lower_x; x < higher_x; x++) {
            if(grid.find(x) != grid.end()){
                if(grid[x].find(y) != grid[x].end()) {
                    if(grid[x][y].isOccupiedNaiveMethod())
                        occupancyGrid.data.push_back(100);
                    else
                        occupancyGrid.data.push_back(0);
                }
                else {
                    occupancyGrid.data.push_back(-1);
                }
            }
            else{
                occupancyGrid.data.push_back(-1);
            }
        }
    }

    occupancyGrid.header.frame_id = frame;
    occupancyGrid.header.stamp = ros::Time::now();
    occupancyGrid.info.origin.position.x = lower_x;
    occupancyGrid.info.origin.position.y = lower_y;
    occupancyGrid.info.origin.position.z = 0;
    occupancyGrid.info.resolution = 1;
    occupancyGrid.info.width = higher_x - lower_x;
    occupancyGrid.info.height = higher_y - lower_y;    
    occupancyGrid.info.origin.orientation.x = 0;
    occupancyGrid.info.origin.orientation.y = 0;
    occupancyGrid.info.origin.orientation.z = 0;
    occupancyGrid.info.origin.orientation.w = 1;

}