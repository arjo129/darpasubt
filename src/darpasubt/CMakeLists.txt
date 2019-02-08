cmake_minimum_required(VERSION 3.5.1)
project(darpasubt)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

catkin_package(CATKIN_DEPENDS 
  roscpp
)
