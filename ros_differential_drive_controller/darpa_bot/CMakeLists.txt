cmake_minimum_required(VERSION 2.8.3)
project(darpa_bot)

find_package(catkin REQUIRED COMPONENTS
  rospy
  message_generation
  nav_msgs
  roscpp
  rosserial_arduino
  rosserial_client
  std_msgs
  tf
)

catkin_package(
  LIBRARIES darpa_bot
  CATKIN_DEPENDS rospy nav_msgs roscpp rosserial_arduino rosserial_client std_msgs tf message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

rosserial_generate_ros_lib(
    PACKAGE rosserial_arduino
    SCRIPT make_libraries.py
)

rosserial_configure_client(
    DIRECTORY firmware
    TOOLCHAIN_FILE ${ROSSERIAL_ARDUINO_TOOLCHAIN}
)

rosserial_add_client_target(firmware bot ALL)
rosserial_add_client_target(firmware bot-upload)

