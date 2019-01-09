#ifndef _IMU_INTEGRATION_H_
#define _IMU_INTEGRATION_H_

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <string>
#include <subt_seed/LocalizationMethod.h>

/**
 * This class provides fusion between IMU and Odometry data thus allowing a 
 * primitive map to be built up.
 */  
class IMUIntegration: LocalizationMethod {
public:
    ros::Subscriber odom, imu;
    ros::NodeHandle nh;
    
    /**
     * Start the IMU integration
     */  
    IMUIntegration(ros::NodeHandle _nh, std::string nodeName);

    /**
     * Callback for IMU data
     */ 
    void onImuDataRecieved(sensor_msgs::Imu data);

    /**
     * Callback for IMU data
     */ 
    void onImuDataRecieved(sensor_msgs::Imu data);
};
#endif