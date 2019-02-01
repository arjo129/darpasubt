#ifndef ODOMETRYBASEDLOCALIZATION_H
#define ODOMETRYBASEDLOCALIZATION_H

#include <string>

#include <boost/shared_ptr.hpp>

#include <Eigen/Eigen>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

/**
 * @brief The OdometryBasedLocalization class -  class manages integrating the odometry data
 */
class OdometryBasedLocalization
{
private:
    ros::Subscriber odometrySub, imuSub;
    boost::shared_ptr<ros::NodeHandle> node;
    nav_msgs::Odometry lastOdom;
    std::string robotName;

    Eigen::Quaterniond orientationInfo;
    Eigen::Vector3d position, prevOdom;
    bool first = true;

    /**
     * @brief onImuRecieveData - Callback for imu data
     * @param imu
     */
    void onImuRecieveData(sensor_msgs::Imu imu);
    /**
     * @brief onOdometryRecieveData - callback for odometry data.
     * Relies on IMU data to recalculate position.
     * @param odom
     */
    void onOdometryRecieveData(nav_msgs::Odometry odom);

public:
    OdometryBasedLocalization(boost::shared_ptr<ros::NodeHandle> nh, std::string botName);

    /**
     * @brief broadcast TF Data
     */
    void broadcast();
};

#endif // ODOMETRYBASEDLOCALIZATION_H
