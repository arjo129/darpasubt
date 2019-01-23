#include <subt_seed/odometrybasedlocalization.h>

OdometryBasedLocalization::OdometryBasedLocalization(boost::shared_ptr<ros::NodeHandle> nh,
                                                     std::string botName)
{
    node = nh;
    robotName = botName;
    imuSub = nh->subscribe(robotName+"/imu/data",1,
                           &OdometryBasedLocalization::onImuRecieveData, this);

}

void OdometryBasedLocalization::onImuRecieveData(sensor_msgs::Imu imu) {
    geometry_msgs::Quaternion qt = imu.orientation;
    orientationInfo.x() = qt.x;
    orientationInfo.y() = qt.y;
    orientationInfo.z() = qt.z;
    orientationInfo.w() = qt.w;
}

void OdometryBasedLocalization::onOdometryRecieveData(nav_msgs::Odometry odom) {

    double cX = odom.pose.pose.position.x;
    double cY = odom.pose.pose.position.y;
    double cZ = odom.pose.pose.position.y;


}

