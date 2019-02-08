#include <ros/ros.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_ros/transforms.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>
#include <tf2_ros/buffer.h>
#include <tf2/transform_datatypes.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <sensor_msgs/PointCloud2.h>
#include <string>
#include <cmath>

class GroundPlaneFilter
{
public:
  GroundPlaneFilter();

private:
  ros::Subscriber cloud_sub_;
  ros::Publisher filtered_cloud_pub_;
  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;
  
  void cloudCallback(const sensor_msgs::PointCloud2::ConstPtr& cloud);
};

GroundPlaneFilter::GroundPlaneFilter()
  : tf_listener_(tf_buffer_)
{
  ros::NodeHandle handle;
  ros::NodeHandle handle_private("~");
  
  cloud_sub_ = handle.subscribe("/X1_1/points", 1, &GroundPlaneFilter::cloudCallback, this);
  filtered_cloud_pub_ = handle.advertise<sensor_msgs::PointCloud2>("/X1_1/filtered_cloud", 1);
}
  
void GroundPlaneFilter::cloudCallback(const sensor_msgs::PointCloud2::ConstPtr& cloud)
{
  // Transforms and conversions.
  geometry_msgs::TransformStamped transform;
  try
  {
    transform = tf_buffer_.lookupTransform("X1_1/base_footprint", cloud->header.frame_id, cloud->header.stamp, ros::Duration(0.1));
  }
  catch (tf2::TransformException& e)
  {
    return;
  }
  sensor_msgs::PointCloud2 cloud_transformed;
  tf2::doTransform(*cloud, cloud_transformed, transform);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl(new pcl::PointCloud<pcl::PointXYZ>); 
  pcl::fromROSMsg(cloud_transformed, *cloud_pcl);

  // Create the segmentation object.
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  seg.setOptimizeCoefficients (true);
  seg.setModelType(pcl::SACMODEL_PERPENDICULAR_PLANE);
  seg.setMethodType(pcl::SAC_RANSAC);
  seg.setMaxIterations(2000);
  seg.setDistanceThreshold(0.5);
  seg.setAxis(Eigen::Vector3f(0,0,1));
  seg.setEpsAngle(0.1);

  // Segmentation.
  pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
  pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
  seg.setInputCloud(cloud_pcl);
  seg.segment(*inliers, *coefficients);

  // Filtering.
  pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_cloud_pcl(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::ExtractIndices<pcl::PointXYZ> extract;
  extract.setInputCloud(cloud_pcl);
  extract.setNegative(true);
  extract.setIndices(inliers);
  extract.filter(*filtered_cloud_pcl);

  // Publish.
  sensor_msgs::PointCloud2 filtered_cloud;
  pcl::toROSMsg(*filtered_cloud_pcl, filtered_cloud);
  filtered_cloud_pub_.publish(filtered_cloud);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "ground_plane_filter");
  GroundPlaneFilter ground_plane_filter;

  ros::spin();
}
