#include <ros/ros.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_ros/transforms.h>
#include <sensor_msgs/PointCloud2.h>
#include <cartographer_ros_msgs/SubmapCloudQuery.h>
#include <cartographer_ros_msgs/SubmapList.h>

class MappingCloudPublisher
{
public:
  MappingCloudPublisher();

private:
  ros::Subscriber submap_list_sub_;
  ros::ServiceClient client_;
  ros::Publisher cloud_pub_;
  ros::Timer timer_;

  std::vector<int> submap_indices_;

  void timerCallback(const ros::TimerEvent& timer);
  void submapListCallback(const cartographer_ros_msgs::SubmapList::ConstPtr submap_list);
};

MappingCloudPublisher::MappingCloudPublisher()
{
  ros::NodeHandle handle;
  ros::NodeHandle handle_private("~");

  submap_list_sub_ = handle.subscribe("/submap_list", 1, &MappingCloudPublisher::submapListCallback, this);
  client_ = handle.serviceClient<cartographer_ros_msgs::SubmapCloudQuery>("submap_cloud_query");
  cloud_pub_ = handle.advertise<sensor_msgs::PointCloud2>("/X1_1/mapping_cloud", 1);
  timer_ = handle.createTimer(ros::Duration(5), &MappingCloudPublisher::timerCallback, this);
}  

void MappingCloudPublisher::timerCallback(const ros::TimerEvent& timer)
{
  pcl::PointCloud<pcl::PointXYZI>::Ptr combined_cloud(new pcl::PointCloud<pcl::PointXYZI>);

  for (int submap_index : submap_indices_)
  {
    cartographer_ros_msgs::SubmapCloudQuery query;
    query.request.trajectory_id = 0;
    query.request.submap_index = submap_index;
    query.request.min_probability = 0.1;
    query.request.high_resolution = true;
    client_.call(query);

    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromROSMsg(query.response.cloud, *cloud);

    *combined_cloud += *cloud;
  }

  sensor_msgs::PointCloud2 combined_cloud_msg;
  pcl::toROSMsg(*combined_cloud, combined_cloud_msg);
  combined_cloud_msg.header.stamp = ros::Time::now();
  combined_cloud_msg.header.frame_id = "map";

  cloud_pub_.publish(combined_cloud_msg);
}

void MappingCloudPublisher::submapListCallback(const cartographer_ros_msgs::SubmapList::ConstPtr submap_list)
{
  submap_indices_.clear();
  for (const auto& entry : submap_list->submap)
  {
    if (entry.trajectory_id == 0)
    {
      submap_indices_.push_back(entry.submap_index);
    }
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mapping_cloud_publisher");
  MappingCloudPublisher mapping_cloud_publisher;

  ros::spin();
}
