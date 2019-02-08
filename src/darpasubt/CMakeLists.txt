cmake_minimum_required(VERSION 3.5.1)
project(darpasubt)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  geometry_msgs
  std_msgs
  pcl_ros
  pcl_conversions
  tf2
  tf2_ros
  tf2_msgs
  tf2_sensor_msgs
  tf2_geometry_msgs
)

catkin_package(CATKIN_DEPENDS 
  roscpp
  sensor_msgs
  geometry_msgs
  std_msgs
  pcl_ros
  pcl_conversions
  tf2
  tf2_ros
  tf2_msgs
  tf2_sensor_msgs
  tf2_geometry_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(ground_plane_filter src/ground_plane_filter.cpp)
target_link_libraries(ground_plane_filter ${catkin_LIBRARIES})
