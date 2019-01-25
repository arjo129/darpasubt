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
#ifndef SUBT_GAZEBO_COMMONTYPES_HH_
#define SUBT_GAZEBO_COMMONTYPES_HH_

#include <map>
#include <memory>
#include <string>
#include <gazebo/common/Time.hh>
#include <gazebo/physics/PhysicsTypes.hh>
#include <ignition/math/graph/Graph.hh>

namespace subt
{
  /// \def Neighbors_M
  /// \brief Map of neighbors
  using Neighbors_M = std::map<std::string, double>;

  /// \def VisibilityGraph
  /// \brief An undirected graph to represent communication visibility between
  /// different areas of the world.
  using VisibilityGraph =
    ignition::math::graph::UndirectedGraph<std::string, uint8_t>;

  /// \brief Class used to store information about a member of the team.
  class TeamMember
  {
    /// \brief Gazebo name used for this model.
    public: std::string name;

    /// \brief Address of the robot. E.g.: 192.168.1.2
    public: std::string address;

    /// \brief Model pointer.
    public: gazebo::physics::ModelPtr model;

    /// \brief List of neighbors and comms probabilities for this robot.
    public: Neighbors_M neighbors;

    /// \brief Is this robot on outage?
    public: bool onOutage;

    /// \brief When will the last outage finish?
    public: gazebo::common::Time onOutageUntil;

    /// \brief Current data rate usage (bits).
    public: uint32_t dataRateUsage;
  };

  /// \brief All the supported artifact types.
  enum class ArtifactType : uint32_t
  {
    TYPE_BACKPACK       = 0,
    TYPE_DUCT           = 1,
    TYPE_ELECTRICAL_BOX = 2,
    TYPE_EXTINGUISHER   = 3,
    TYPE_PHONE          = 4,
    TYPE_RADIO          = 5,
    TYPE_TOOLBOX        = 6,
    TYPE_VALVE          = 7,
  };

  /// \def TeamMemberPtr
  /// \brief Shared pointer to TeamMember
  using TeamMemberPtr = std::shared_ptr<TeamMember>;

  /// \def TeamMembership_M
  /// \brief Map containing information about the members of the team.
  /// The key is the robot address. The value is a pointer to a TeamMember
  /// object that contains multiple information about the robot.
  using TeamMembership_M = std::map<std::string, TeamMemberPtr>;

  /// \def TeamMembershipPtr
  /// \brief A shared pointer to the membership data structure.
  /// \sa TeamMembership_M
  using TeamMembershipPtr = std::shared_ptr<TeamMembership_M>;

  /// \brief Address used to send a message to all the members of the team
  /// listening on a specific port.
  const std::string kBroadcast = "broadcast";

  /// \brief Address used to bind to a multicast group. Note that we do not
  /// support multiple multicast groups, only one.
  const std::string kMulticast = "multicast";

  /// \brief Service used to centralize all messages sent from the agents.
  const std::string kBrokerSrv = "broker";

  /// \brief Service used to validate an address.
  const std::string kAddrRegistrationSrv = "address/register";

  /// \brief Service used to invalidate an address.
  const std::string kAddrUnregistrationSrv = "address/unregister";

  /// \brief Service used to register an end point.
  const std::string kEndPointRegistrationSrv = "end_point/register";

  /// \brief Address used to receive neighbor updates.
  const std::string kNeighborsTopic = "neighbors";

  /// \brief The name of the base station in Gazebo. This is used as the
  /// address to report artifacts.
  const std::string kBaseStationName = "BaseStation";

  /// \brief Internal service used to report new artifacts.
  const std::string kNewArtifactSrv = "/subt/artifacts/new";

  /// \brief Default port.
  const uint32_t kDefaultPort = 4100u;

  /// \brief Scoped name of collision which detects an entering object in the
  /// start area to initiate the game.
  const std::string kStartCollisionName  = "start_area::link::collision";
}
#endif
