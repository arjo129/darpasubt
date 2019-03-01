cmake_minimum_required(VERSION 2.8.12)
add_compile_options(-std=c++11)
project(ynav)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
  tf
  pcl_ros
  pcl_conversions)

catkin_package(
  CATKIN_DEPENDS
  roscpp
  sensor_msgs
  std_msgs
  tf
  pcl_ros
  pcl_conversions
  INCLUDE_DIRS include
  LIBRARIES ynav_lib)

include_directories(include ${catkin_INCLUDE_DIRS})

include_directories("/usr/include/ompl")
find_library(ompl_LIBRARIES NAMES ompl)

install(DIRECTORY include/ynav/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h")

add_library(ynav_lib 
  src/ynav/point_index.cpp 
  src/ynav/astar/coordinates.cpp 
  src/ynav/astar/node.cpp
  src/ynav/astar/tree.cpp 
  src/ynav/astar/heuristic.cpp)
target_link_libraries(ynav_lib ${ompl_LIBRARIES} ${catkin_LIBRARIES})

install(TARGETS ynav_lib
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
