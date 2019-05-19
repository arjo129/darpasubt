cmake_minimum_required(VERSION 2.8.3)

include_directories(${ROS_LIB_DIR})

add_definitions(-DUSB_CON})

link_directories(~/Arduino/libraries)

generate_arduino_firmware(bot
    SRCS movement.cpp ${ROS_LIB_DIR}/time.cpp
    BOARD uno
    PORT /dev/ttyUSB0
)
