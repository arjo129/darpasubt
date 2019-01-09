cmake_minimum_required(VERSION 3.5.1)
project(subt_seed)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
endif()

set (SUBT_MAJOR_VERSION 0)
set (SUBT_MINOR_VERSION 1)
set (SUBT_PATCH_VERSION 0)
set (SUBT_VERSION_FULL
  ${SUBT_MAJOR_VERSION}.${SUBT_MINOR_VERSION}.${SUBT_PATCH_VERSION})

find_package(catkin REQUIRED
  COMPONENTS
    message_generation
    subt_gazebo
    roscpp
    pcl_ros
    tf2
    octomap
    octomap_ros
)

find_package(gazebo REQUIRED)

find_package(PCL REQUIRED COMPONENTS common io)

find_package(octomap REQUIRED)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

########################
## Message generation ##
########################

# Add here SubT ROS messages.

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
  ${OCTOMAP_INCLUDE_DIRS}
)

link_directories(
  ${GAZEBO_LIBRARY_DIRS}
)

## Declare a C++ executable
add_executable(
  subt_seed_node
  src/subt_seed_node.cc
  src/localmap.cpp
  src/localmap3d.cpp
)

add_dependencies(
  subt_seed_node
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(
  subt_seed_node
  ${catkin_LIBRARIES}
  ${PCL_COMMON_LIBRARIES}
  ${OCTOMAP_LIBRARIES}
)

###########
## Tests ##
###########
catkin_add_gtest(probabilityTest test/ProbabilityDistributionTest.cpp)
target_link_libraries(
  probabilityTest
  ${catkin_LIBRARIES}
  ${PCL_COMMON_LIBRARIES}
  ${OCTOMAP_LIBRARIES}
)


#############
## Install ##
#############

# Install all the launch files
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch)

# Mark executables and/or libraries for installation
install(TARGETS subt_seed_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)