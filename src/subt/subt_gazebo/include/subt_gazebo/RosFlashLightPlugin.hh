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

#ifndef GAZEBO_PLUGINS_ROSFLASHLIGHTPLUGIN_HH_
#define GAZEBO_PLUGINS_ROSFLASHLIGHTPLUGIN_HH_

#include <ros/ros.h>
#include <std_srvs/SetBool.h>

#include <gazebo/plugins/LedPlugin.hh>

namespace gazebo
{
  /// \brief A plugin providing a ROS service to turn on/off lights attached on
  /// a model in the environment.
  ///
  /// The default ROS service name is "light_control", and its parameter is a
  /// boolean.
  /// When it is given with true by a ROS service call,
  ///
  ///   i.e., "rosservice /light_control true",
  ///
  /// the lights are turned on. In the case of false, they are turned off.
  ///
  /// This plugin inherits from LedPlugin. While the format of parameters
  /// follows that of the base plugin, it also accepts <service_name>,
  /// to change the ROS service name.
  ///
  /// Example:
  /// <service_name>light_control1</service_name>
  /// <enable>true</enable>
  /// <light>
  ///  <id>link1/light_source</id>
  ///  <duration>0.1</duration>
  ///  <interval>0.4</interval>
  ///  <enable>true</enable>
  /// </light>
  /// ...
  ///
  class RosFlashLightPlugin : public LedPlugin
  {
    // Documentation inherited.
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) override;

    /// \brief Called by ROS service client.
    /// \param[in] _req A request to turn on/off the lights.
    /// \param[in] _res A response to indicate success or failure.
    /// \return True if the order is successful.
    public: virtual bool Control(
      std_srvs::SetBool::Request &_req, std_srvs::SetBool::Response &_res);

    /// \brief ROS service server.
    private: ros::ServiceServer service;
  };
}
#endif
