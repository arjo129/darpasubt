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

#include <algorithm>
#include <memory>
#include <iostream>
#include <gazebo/physics/PhysicsIface.hh>
#include <gazebo/physics/World.hh>
#include <ignition/math/Rand.hh>
#include "subt_gazebo/protobuf/datagram.pb.h"
#include "subt_gazebo/protobuf/neighbor_m.pb.h"
#include "subt_gazebo/Broker.hh"
#include "subt_gazebo/CommonTypes.hh"

using namespace subt;

//////////////////////////////////////////////////
Broker::Broker()
  : team(std::make_shared<TeamMembership_M>()),
    rndEngine(std::default_random_engine(ignition::math::Rand::Seed()))
{
  // Advertise the service for registering addresses.
  if (!this->node.Advertise(kAddrRegistrationSrv,
        &Broker::OnAddrRegistration, this))
  {
    std::cerr << "Error advertising srv [" << kAddrRegistrationSrv << "]"
              << std::endl;
    return;
  }

  // Advertise the service for unregistering addresses.
  if (!this->node.Advertise(kAddrUnregistrationSrv,
        &Broker::OnAddrUnregistration, this))
  {
    std::cerr << "Error advertising srv [" << kAddrUnregistrationSrv << "]"
              << std::endl;
    return;
  }

  // Advertise the service for registering end points.
  if (!this->node.Advertise(kEndPointRegistrationSrv,
        &Broker::OnEndPointRegistration, this))
  {
    std::cerr << "Error advertising srv [" << kEndPointRegistrationSrv << "]"
              << std::endl;
    return;
  }

  // Advertise a oneway service for centralizing all message requests.
  if (!this->node.Advertise(kBrokerSrv, &Broker::OnMessage, this))
  {
    std::cerr << "Error advertising srv [" << kBrokerSrv << "]" << std::endl;
    return;
  }

  // Advertise a topic for notifying neighbor updates.
  this->neighborPub =
    this->node.Advertise<subt::msgs::Neighbor_M>(kNeighborsTopic);
  if (!this->neighborPub)
  {
    std::cerr << "Error advertising topic [" << kNeighborsTopic << "]"
              << std::endl;
    return;
  }
}

//////////////////////////////////////////////////
void Broker::Reset()
{
  std::lock_guard<std::mutex> lk(this->mutex);
  this->incomingMsgs.clear();
  this->endpoints.clear();
}

//////////////////////////////////////////////////
subt::TeamMembershipPtr Broker::Team()
{
  return this->team;
}

//////////////////////////////////////////////////
void Broker::NotifyNeighbors()
{
  subt::msgs::Neighbor_M neighbors;

  // Send neighbors updates to each member of the team.
  for (auto const &robot : (*this->Team()))
  {
    auto address = robot.first;
    auto teamMember = (*this->Team())[address];

    // Populate the list of neighbors for this address.
    ignition::msgs::StringMsg_V v;
    for (auto const &neighbor : teamMember->neighbors)
      v.add_data(neighbor.first);

    // Add the list of neighbors for each address.
    (*neighbors.mutable_neighbors())[address] = v;
  }

  // Notify all clients the updated list of neighbors.
  if (!this->neighborPub.Publish(neighbors))
    std::cerr << "[Broker::NotifyNeighbors(): Error on update" << std::endl;
}

//////////////////////////////////////////////////
void Broker::DispatchMessages(const uint32_t _maxDataRatePerCycle,
    const uint16_t _udpOverhead)
{
  // Shuffle the messages.
  std::shuffle(this->incomingMsgs.begin(), this->incomingMsgs.end(),
      this->rndEngine);

  // Clear the data rate usage for each robot.
  for (const auto &member : (*this->team))
    member.second->dataRateUsage = 0;

  while (!this->incomingMsgs.empty())
  {
    // Get the next message to dispatch.
    const subt::msgs::Datagram msg = this->incomingMsgs.front();
    this->incomingMsgs.pop_front();

    // Sanity check: Make sure that the sender is a member of the team.
    if (this->team->find(msg.src_address()) == this->team->end())
    {
      std::cerr << "Broker::DispatchMessages(): Discarding message. Robot ["
                << msg.src_address() << "] is not registered as a member of the"
                << " team" << std::endl;
      continue;
    }

    // Get the list of neighbors of the sender.
    const Neighbors_M &neighbors = (*this->team)[msg.src_address()]->neighbors;

    // Update the data rate usage.
    auto dataSize = (msg.data().size() + _udpOverhead) * 8;
    (*this->team)[msg.src_address()]->dataRateUsage += dataSize;
    for (const auto &neighbor : neighbors)
    {
      // We account the overhead caused by the UDP/IP/Ethernet headers + the
      // payload. We convert the total amount of bytes to bits.
      (*this->team)[neighbor.first]->dataRateUsage += dataSize;
    }

    std::string dstEndPoint =
      msg.dst_address() + ":" + std::to_string(msg.dst_port());
    if (this->endpoints.find(dstEndPoint) != this->endpoints.end())
    {
      // Shuffle the clients bound to this endpoint.
      std::vector<BrokerClientInfo> clientsV = this->endpoints.at(dstEndPoint);
      std::shuffle(clientsV.begin(), clientsV.end(), this->rndEngine);

      for (const BrokerClientInfo &client : clientsV)
      {
        // Make sure that we're sending the message to a valid neighbor.
        if (neighbors.find(client.address) == neighbors.end())
          continue;

        // Check if the maximum data rate has been reached in the destination.
        if ((*team)[client.address]->dataRateUsage > _maxDataRatePerCycle)
        {
          // Debug output
          // gzdbg << "Dropping message (max data rate) from "
          //       << msg.src_address() << " to " << client.address
          //       << " (addressed to " << msg.dst_address()
          //       << ")" << std::endl;
          continue;
        }

        // Decide whether this neighbor gets this message, according to the
        // probability of communication between them right now.
        const double &neighborProb = neighbors.at(client.address);
        if (ignition::math::Rand::DblUniform(0.0, 1.0) < neighborProb)
        {
          // Debug output
          // std::cout << "Sending message from " << msg.src_address() << " to "
          //           << client.address << " (addressed to "
          //           << msg.dst_address() << ")" << std::endl;
          if (!this->node.Request(client.address, msg))
          {
            std::cerr << "[CommsBrokerPlugin::DispatchMessages()]: Error "
                      << "sending message to [" << client.address << "]"
                      << std::endl;
          }
        }
        // else
        // {
        //   // Debug output.
        //   gzdbg << "Dropping message from " << msg.src_address() << " to "
        //         << client.address << " (addressed to " << msg.dst_address()
        //         << ")" << std::endl;
        // }
      }
    }
  }
}

/////////////////////////////////////////////////
std::mutex &Broker::Mutex()
{
  return this->mutex;
}

//////////////////////////////////////////////////
bool Broker::Bind(const std::string &_clientAddress,
  const std::string &_endpoint)
{
  // Make sure that the same client didn't bind the same end point before.
  if (this->endpoints.find(_endpoint) != this->endpoints.end())
  {
    const auto &clientsV = this->endpoints[_endpoint];
    for (const auto &client : clientsV)
    {
      if (client.address == _clientAddress)
      {
        std::cerr << "Broker::Bind() error: Address [" << _clientAddress
                  << "] already used in a previous Bind()" << std::endl;
        return false;
      }
    }
  }

  BrokerClientInfo clientInfo;
  clientInfo.address = _clientAddress;
  this->endpoints[_endpoint].push_back(clientInfo);

  std::cout << "New endpoint registered [" << _endpoint << "] for client ["
            << _clientAddress << "]" << std::endl;

  return true;
}

//////////////////////////////////////////////////
bool Broker::Register(const std::string &_id)
{
  if (this->team->find(_id) != this->team->end())
  {
    std::cerr << "Logger::Register() error: ID [" << _id << "] already exists"
              << std::endl;
    return false;
  }

  auto const &model = gazebo::physics::get_world()->ModelByName(_id);
  if (!model)
    return false;

  auto newMember = std::make_shared<TeamMember>();

  // Name and address are the same in SubT.
  newMember->address = _id;
  newMember->name = _id;
  newMember->model = model;
  (*this->team)[_id] = newMember;

  std::cout << "New client registered [" << _id << "]" <<  std::endl;

  return true;
}

//////////////////////////////////////////////////
bool Broker::Unregister(const std::string &_id)
{
  // Sanity check: Make sure that the ID exists.
  if (this->team->find(_id) == this->team->end())
  {
    std::cerr << "Broker::Unregister() error: ID [" << _id << "] doesn't exist"
              << std::endl;
    return false;
  }

  this->team->erase(_id);

  // Unbind.
  for (auto &endpointKv : this->endpoints)
  {
    auto &clientsV = endpointKv.second;

    auto i = std::begin(clientsV);
    while (i != std::end(clientsV))
    {
      if (i->address == _id)
        i = clientsV.erase(i);
      else
        ++i;
    }
  }

  return true;
}

/////////////////////////////////////////////////
bool Broker::OnAddrRegistration(const ignition::msgs::StringMsg &_req,
    ignition::msgs::Boolean &_rep)
{
  std::string address = _req.data();
  bool result;

  {
    std::lock_guard<std::mutex> lk(this->mutex);
    result = this->Register(address);
  }

  _rep.set_data(result);

  return result;
}

/////////////////////////////////////////////////
bool Broker::OnAddrUnregistration(const ignition::msgs::StringMsg &_req,
    ignition::msgs::Boolean &_rep)
{
  std::string address = _req.data();
  bool result;

  {
    std::lock_guard<std::mutex> lk(this->mutex);
    result = this->Unregister(address);
  }

  _rep.set_data(result);

  return result;
}

/////////////////////////////////////////////////
bool Broker::OnEndPointRegistration(const ignition::msgs::StringMsg_V &_req,
    ignition::msgs::Boolean &_rep)
{
  if (_req.data().size() != 2)
  {
    std::cerr << "[Broker::OnEndPointRegistration()] Expected two strings and "
              << "got " << _req.data().size() << " instead" << std::endl;
    return false;
  }

  bool result;
  std::string clientAddress = _req.data(0);
  std::string endpoint = _req.data(1);

  {
    std::lock_guard<std::mutex> lk(this->mutex);
    result = this->Bind(clientAddress, endpoint);
  }

  _rep.set_data(result);

  return result;
}

/////////////////////////////////////////////////
void Broker::OnMessage(const subt::msgs::Datagram &_req)
{
  // Just save the message, it will be processed later.
  std::lock_guard<std::mutex> lk(this->mutex);

  // Save the message.
  this->incomingMsgs.push_back(_req);
}
