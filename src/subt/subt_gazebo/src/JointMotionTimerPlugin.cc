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

#include <functional>
#include <memory>
#include <vector>
#include <boost/weak_ptr.hpp>  // NOLINT
#include <ignition/common/Time.hh>
#include <ignition/msgs.hh>
#include <ignition/transport/Node.hh>
#include <gazebo/common/Events.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/PhysicsEngine.hh>
#include <gazebo/physics/World.hh>
#include "subt_gazebo/JointMotionTimerPlugin.hh"

namespace subt
{
GZ_REGISTER_MODEL_PLUGIN(JointMotionTimerPlugin)

using JointWeakPtr = boost::weak_ptr<gazebo::physics::Joint>;

/// \internal
/// \brief Private data for the JointMotionTimerPlugin class.
class JointMotionTimerPluginPrivate
{
  /// \brief Joint velocity threshold.
  public: double velocityThreshold = 1e-2;

  /// \brief Elapsed time.
  public: ignition::common::Time elapsedTime;

  /// \brief PhysicsEngine pointer for getting timestep.
  public: gazebo::physics::PhysicsEnginePtr engine;

  /// \brief Pointer to the model that defines this plugin
  public: gazebo::physics::ModelPtr model;

  /// \brief Joints to check for motion.
  public: std::vector<JointWeakPtr> joints;

  /// \brief Ignition transport node for communications.
  public: std::unique_ptr<ignition::transport::Node> node;

  /// \brief Ignition transport publisher for elapsed time.
  public: ignition::transport::Node::Publisher elapsedTimePub;

  /// \brief Ignition transport publisher for elapsed time updates.
  public: ignition::transport::Node::Publisher elapsedTimeUpdatesPub;

  /// \brief World update connection pointer.
  public: gazebo::event::ConnectionPtr updateConnection;
};

/////////////////////////////////////////////////
JointMotionTimerPlugin::JointMotionTimerPlugin()
  : dataPtr(new JointMotionTimerPluginPrivate)
{
}

/////////////////////////////////////////////////
JointMotionTimerPlugin::~JointMotionTimerPlugin()
{
}

/////////////////////////////////////////////////
void JointMotionTimerPlugin::Load(gazebo::physics::ModelPtr _parent,
                                  sdf::ElementPtr _sdf)
{
  this->dataPtr->model = _parent;

  auto world = _parent->GetWorld();
  if (!world)
  {
    gzerr << "Unable to get world pointer." << std::endl;
    return;
  }
  this->dataPtr->engine = world->Physics();
  if (!this->dataPtr->engine)
  {
    gzerr << "Unable to get physics engine pointer." << std::endl;
    return;
  }

  if (_sdf->HasElement("joint_velocity_threshold"))
  {
    this->dataPtr->velocityThreshold =
      _sdf->Get<double>("joint_velocity_threshold");
  }

  // Get joints specified in sdf
  if (_sdf->HasElement("all_joints"))
  {
    auto joints = _parent->GetJoints();
    for (const auto &joint : joints)
    {
      if (!joint->HasType(gazebo::physics::Base::FIXED_JOINT))
      {
        this->dataPtr->joints.push_back(JointWeakPtr(joint));
      }
    }
  }
  else if (_sdf->HasElement("joint"))
  {
    auto elem = _sdf->GetElement("joint");
    while (elem)
    {
      const std::string jointName = elem->Get<std::string>();
      auto joint = this->dataPtr->model->GetJoint(jointName);
      if (!joint)
      {
        gzerr << "Could not find joint [" << jointName << "]." << std::endl;
      }
      else if (joint->HasType(gazebo::physics::Base::FIXED_JOINT))
      {
        gzerr << "Joint [" << jointName << "] is a fixed joint, skipping."
              << std::endl;
      }
      else
      {
        this->dataPtr->joints.push_back(JointWeakPtr(joint));
      }
      elem = elem->GetNextElement("joint");
    }
  }

  ignition::transport::NodeOptions nodeOptions;
  if (_sdf->HasElement("robotNamespace"))
  {
    const std::string robotNamespace = _sdf->Get<std::string>("robotNamespace");
    if (!nodeOptions.SetNameSpace(robotNamespace))
    {
      gzerr << "Unable to set the ignition transport namespace: "
            << robotNamespace
            << std::endl;
    }
  }
  this->dataPtr->node.reset(new ignition::transport::Node(nodeOptions));

  this->dataPtr->elapsedTimePub =
    this->dataPtr->node->Advertise<ignition::msgs::Duration>(
      "elapsed_time");

  this->dataPtr->elapsedTimeUpdatesPub =
    this->dataPtr->node->Advertise<ignition::msgs::Duration>(
      "elapsed_time_updates");

  this->dataPtr->updateConnection =
      gazebo::event::Events::ConnectWorldUpdateBegin(
          std::bind(&JointMotionTimerPlugin::OnUpdate, this));
}

/////////////////////////////////////////////////
ignition::common::Time JointMotionTimerPlugin::ElapsedTime() const
{
  return this->dataPtr->elapsedTime;
}

/////////////////////////////////////////////////
std::vector<gazebo::physics::JointPtr> JointMotionTimerPlugin::Joints() const
{
  std::vector<gazebo::physics::JointPtr> joints;
  for (const auto &weakJoint : this->dataPtr->joints)
  {
    gazebo::physics::JointPtr joint = weakJoint.lock();
    if (joint)
    {
      joints.push_back(joint);
    }
  }
  return joints;
}

/////////////////////////////////////////////////
void JointMotionTimerPlugin::OnUpdate()
{
  ignition::common::Time dt(this->dataPtr->engine->GetMaxStepSize());

  bool motionDetected = false;
  for (const auto &joint : this->Joints())
  {
    if (std::abs(joint->GetVelocity(0)) > this->dataPtr->velocityThreshold)
    {
      motionDetected = true;
      break;
    }
  }

  if (motionDetected)
  {
    this->dataPtr->elapsedTime += dt;
  }

  ignition::msgs::Duration msg;
  msg.set_sec(this->dataPtr->elapsedTime.sec);
  msg.set_nsec(this->dataPtr->elapsedTime.nsec);
  this->dataPtr->elapsedTimePub.Publish(msg);
  if (motionDetected)
  {
    this->dataPtr->elapsedTimeUpdatesPub.Publish(msg);
  }
}
}
