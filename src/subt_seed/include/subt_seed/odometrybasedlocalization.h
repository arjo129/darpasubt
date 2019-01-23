#ifndef ODOMETRYBASEDLOCALIZATION_H
#define ODOMETRYBASEDLOCALIZATION_H

#include <string>

#include <boost/shared_ptr.hpp>

#include <Eigen/Eigen>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>

#include <ros/ros.h>
#include <tf/tf.h>
/**
 * @brief The OdometryBasedLocalization class -  class manages integrating the odometry data
 */
class OdometryBasedLocalization
{
public:
    ros::Subscriber odometrySub, imuSub;
    boost::shared_ptr<ros::NodeHandle> node;
    nav_msgs::Odometry lastOdom;
    std::string robotName;

    Eigen::Quaterniond orientationInfo;
    Eigen::Vector3d position;
    nav_msgs::Odometry prevOdom;

    OdometryBasedLocalization(boost::shared_ptr<ros::NodeHandle> nh, std::string botName);
    void onImuRecieveData(sensor_msgs::Imu imu);
    void onOdometryRecieveData(nav_msgs::Odometry odom);
};

#endif // ODOMETRYBASEDLOCALIZATION_H
