/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <geometry_msgs/Twist.h>
#include <sensor_msgs/PointCloud2.h>
#include <ros/ros.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl_ros/transforms.h>
#include <nav_msgs/Odometry.h>

#include <string>

#include <subt_gazebo/CommsClient.hh>
#include <subt_seed/mapper.h>

#include <tf/transform_broadcaster.h>

/// \brief. Example control class, running as a ROS node to control a robot.
class Controller
{
  /// \brief Constructor.
  /// The controller uses the given name as a prefix of its topics, e.g.,
  /// "/x1/cmd_vel" if _name is specified as "x1".
  /// \param[in] _name Name of the robot.
  public: Controller(const std::string &_name);

  /// \brief A function that will be called every loop of the ros spin
  /// cycle.
  public: void Update();

  /// \brief Callback function for message from other comm clients.
  /// \param[in] _srcAddress The address of the robot who sent the packet.
  /// \param[in] _dstAddress The address of the robot who received the packet.
  /// \param[in] _dstPort The destination port.
  /// \param[in] _data The contents the packet holds.
  private: void CommClientCallback(const std::string &_srcAddress,
                                   const std::string &_dstAddress,
                                   const uint32_t _dstPort,
                                   const std::string &_data);

  /// \brief ROS node handler.
  private: ros::NodeHandle n;
  tf::TransformListener listener;
  LocalMap* localMap;
  //LocalMap3d* localMap3d;
  std::string name;
  octomap::point3d* sensor_origin;

  private: void handlePointCloud(const sensor_msgs::PointCloud2ConstPtr& cloud_msg);
  void handleOdom(const nav_msgs::Odometry& odom_msg);

  /// \brief publisher to send cmd_vel
  private: ros::Publisher velPub, mapPub, map3dPub;

  private: ros::Subscriber sub;
  ros::Subscriber odomSub;

  /// \brief Communication client.
  private: std::unique_ptr<subt::CommsClient> client;
};

/////////////////////////////////////////////////
Controller::Controller(const std::string &_name)
{
  name = _name;
  // Create subt communication client
  this->client.reset(new subt::CommsClient(_name));
  this->client->Bind(&Controller::CommClientCallback, this);

  // Create a cmd_vel publisher to control a vehicle.
  this->velPub = this->n.advertise<geometry_msgs::Twist>(_name + "/cmd_vel", 1);
  this->mapPub = this->n.advertise<nav_msgs::OccupancyGrid>(_name+"/local_map",1);
  this->map3dPub = this->n.advertise<octomap_msgs::Octomap>(_name+"/local_map_3d",1);
  this->sub = this->n.subscribe(_name + "/points", 1, &Controller::handlePointCloud, this);
  this->localMap = new LocalMap(_name+"/base_link");
  //this->localMap3d = new LocalMap3d(_name+"/base_link");
  this->odomSub = this->n.subscribe(_name + "/x1_velocity_controller/odom", 1, &Controller::handleOdom, this);

  ROS_INFO("Initiallizing call backs");
}

/////////////////////////////////////////////////
void Controller::CommClientCallback(const std::string &/*_srcAddress*/,
                                    const std::string &/*_dstAddress*/,
                                    const uint32_t /*_dstPort*/,
                                    const std::string &/*_data*/)
{
  // Add code to handle communication callbacks.
  ROS_INFO("CommClientCallback");
}

void Controller::handlePointCloud(const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {
  pcl::PCLPointCloud2 pcl_pc2;
  octomap_msgs::Octomap octomap_msg;
  pcl_conversions::toPCL(*cloud_msg, pcl_pc2);
  pcl::PointCloud<pcl::PointXYZI>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZI>);
  pcl::fromPCLPointCloud2(pcl_pc2, *temp_cloud);
  localMap->update(*temp_cloud);
  //localMap3d->insert(cloud_msg, *(this->sensor_origin));
  //localMap3d->update();
  //octomap_msgs::fullMapToMsg(*localMap3d->tree, octomap_msg);
  //this->map3dPub.publish(octomap_msg);
  nav_msgs::OccupancyGrid grid;
  localMap->toOccupancyGrid(grid);
  this->mapPub.publish(grid);
}

void Controller::handleOdom(const nav_msgs::Odometry& odom_msg) {
  this->sensor_origin = new octomap::point3d(
    odom_msg.pose.pose.position.x,
    odom_msg.pose.pose.position.y,
    odom_msg.pose.pose.position.z);

  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(odom_msg.pose.pose.position.x, odom_msg.pose.pose.position.y, 0.0) );
  tf::Quaternion q(odom_msg.pose.pose.orientation.x, odom_msg.pose.pose.orientation.y, odom_msg.pose.pose.orientation.z, odom_msg.pose.pose.orientation.w);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", name+"/base_link"));
}

/////////////////////////////////////////////////
void Controller::Update()
{
  // Add code that should be processed every iteration.

  // The following code will continuous drive the robot forward.
  geometry_msgs::Twist msg;
  msg.linear.x = 1.0;
  this->velPub.publish(msg);
}

/////////////////////////////////////////////////
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    ROS_ERROR_STREAM("Needs an argument for the competitor's name.");
    return -1;
  }

  // Initialize ros
  ros::init(argc, argv, argv[1]);

  ROS_INFO("Starting seed competitor\n");

  // Create the controller
  Controller controller(argv[1]);

  // This sample code iteratively calls Controller::Update. This is just an
  // example. You can write your controller using alternative methods.
  // To get started with ROS visit: http://wiki.ros.org/ROS/Tutorials
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    controller.Update();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
