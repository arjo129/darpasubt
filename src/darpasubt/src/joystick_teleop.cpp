#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

class JoystickTeleop
{
public:
  JoystickTeleop();

private:
  ros::Subscriber joy_sub_;
  ros::Publisher cmd_vel_pub_;
  ros::Timer timer_;

  int axis_linear_;
  int axis_angular_;
  double scale_linear_;
  double scale_angular_;

  double linear_;
  double angular_;

  void timerCallback(const ros::TimerEvent& event);
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void publish();
};

JoystickTeleop::JoystickTeleop()
{
  ros::NodeHandle nh;
  ros::NodeHandle nh_private("~");
  double frequency;
  nh_private.getParam("frequency", frequency);
  nh_private.getParam("axis_linear", axis_linear_);
  nh_private.getParam("axis_angular", axis_angular_);
  nh_private.param("scale_linear", scale_linear_, 1.0);
  nh_private.param("scale_angular", scale_angular_, 1.0);

  joy_sub_ = nh.subscribe<sensor_msgs::Joy>("/joy", 1, &JoystickTeleop::joyCallback, this);
  cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  timer_ = nh.createTimer(ros::Duration(1 / frequency), &JoystickTeleop::timerCallback, this);
      
  geometry_msgs::Twist cv;
  cmd_vel_pub_.publish(cv);
}

void JoystickTeleop::publish()
{
  geometry_msgs::Twist cmd_vel;
  cmd_vel.linear.x = linear_;
  cmd_vel.angular.z = angular_ * (linear_ >= 0 ? 1 : -1);

  cmd_vel_pub_.publish(cmd_vel);
}

void JoystickTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  linear_ = scale_linear_ * (joy->axes[axis_linear_]);
  angular_ = scale_angular_ * (joy->axes[axis_angular_]);

  publish();
}

void JoystickTeleop::timerCallback(const ros::TimerEvent& event)
{
  publish();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joystick_teleop");
  JoystickTeleop joystick_teleop;

  ros::spin();
}
