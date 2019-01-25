#include <subt_seed/odometrybasedlocalization.h>

OdometryBasedLocalization::OdometryBasedLocalization(boost::shared_ptr<ros::NodeHandle> nh,
                                                     std::string botName)
{
    node = nh;
    robotName = botName;
    imuSub = nh->subscribe(robotName+"/imu/data",1,
                           &OdometryBasedLocalization::onImuRecieveData, this);
    odometrySub = nh->subscribe(robotName+"/x1_velocity_controller/odom",1,
                                &OdometryBasedLocalization::onOdometryRecieveData, this);

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
    double cZ = odom.pose.pose.position.z;

    if(first) {
        prevOdom = Eigen::Vector3d(cX, cY, cZ);
        first = false;
        return ;
    }

    Eigen::Vector3d currentOdom(cX, cY, cZ);
    Eigen::Vector3d delta = currentOdom - prevOdom;
    Eigen::Vector3d reprojectedVector(delta.norm(), 0, 0);
    Eigen::Vector3d velocity = orientationInfo*reprojectedVector;
    position += velocity;
    prevOdom = currentOdom;
}

void OdometryBasedLocalization::broadcast() {

    if(first) return;

    static tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(position.x(), position.y(), position.z()) );
    tf::Quaternion q(orientationInfo.x(), orientationInfo.y(), orientationInfo.z(), orientationInfo.w());
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", robotName+"/base_link"));

}
