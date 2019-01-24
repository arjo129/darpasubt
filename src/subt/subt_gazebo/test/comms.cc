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

#include <gtest/gtest.h>
#include <ros/ros.h>
#include <chrono>

#include "subt_gazebo/CommonTypes.hh"
#include "subt_gazebo/CommsClient.hh"
#include "test/test_config.h"
#include "TestUtils.hh"

using namespace subt;

/////////////////////////////////////////////////
/// \brief A fixture class for testing the CommsBroker plugin within Gazebo and
/// clients using the CommsClient library.
class CommsTest : public testing::Test, public subt::GazeboTest
{
  /// \brief Constructor.
  public: CommsTest()
  {
    // Wait until Gazebo is ready.
    using namespace std::chrono_literals;
    EXPECT_TRUE(this->WaitForGazebo(120s));

    this->client1.reset(new CommsClient(this->addr1));
    this->client2.reset(new CommsClient(this->addr2));

    EXPECT_TRUE(this->client1->Bind(&CommsTest::OnUnicastMsg, this));
    EXPECT_TRUE(this->client1->Bind(&CommsTest::OnMulticastMsg, this,
      kMulticast));
  }

  /// \brief Reset the member variables used for checking test expectations.
  protected: void Reset()
  {
    this->SetUp();
  }

  /// \brief Test different user errors.
  protected: void TestBadUsage()
  {
    {
      // An empty address is not allowed.
      CommsClient client("");
      EXPECT_FALSE(client.Bind(&CommsTest::OnUnicastMsg, this));
      EXPECT_FALSE(client.Bind(&CommsTest::OnMulticastMsg, this, kMulticast));
    }

    {
      // Bind() to an address that is not local is not allowed.
      CommsClient client(this->addr1);
      EXPECT_FALSE(client.Bind(&CommsTest::OnUnicastMsg, this, "other"));
    }

    {
      // A double Bind() is not allowed.
      std::string addr = getRandomNumber();
      CommsClient client(addr);
      EXPECT_TRUE(client.Bind(&CommsTest::OnUnicastMsg, this));
      EXPECT_FALSE(client.Bind(&CommsTest::OnUnicastMsg, this));
      EXPECT_TRUE(client.Bind(&CommsTest::OnMulticastMsg, this, kMulticast));
      EXPECT_FALSE(client.Bind(&CommsTest::OnMulticastMsg, this, kMulticast));
    }
  }

  /// \brief Callback registered for receiving unicast (and broadcast) messages.
  private: void OnUnicastMsg(const std::string &_srcAddress,
                             const std::string &_dstAddress,
                             const uint32_t _dstPort,
                             const std::string &_data)
  {
    this->unicastCallbackExecuted = true;
    EXPECT_EQ(this->addr2, _srcAddress);
    EXPECT_TRUE(_dstAddress == this->addr1 || _dstAddress == kBroadcast);
    EXPECT_EQ(kDefaultPort, _dstPort);
    EXPECT_EQ("_data_", _data);
  }

  /// \brief Callback registered for receiving multicast messages.
  private: void OnMulticastMsg(const std::string &_srcAddress,
                               const std::string &_dstAddress,
                               const uint32_t _dstPort,
                               const std::string &_data)
  {
    this->multicastCallbackExecuted = true;
    EXPECT_EQ(this->addr2, _srcAddress);
    EXPECT_EQ(kMulticast, _dstAddress);
    EXPECT_EQ(kDefaultPort, _dstPort);
    EXPECT_EQ("_data_", _data);
  }

  /// \brief Documentation inherited.
  private: virtual void SetUp()
  {
    this->unicastCallbackExecuted   = false;
    this->multicastCallbackExecuted = false;
  }

  /// \brief Whether a unicast/broadcast message has been received or not.
  protected: bool unicastCallbackExecuted = false;

  /// \brief Whether a multicast message has been received or not.
  protected: bool multicastCallbackExecuted = false;

  protected: std::string addr1 = getRandomNumber();

  protected: std::string addr2 = getRandomNumber();

  /// \brief A comms client.
  protected: std::unique_ptr<CommsClient> client1;

  /// \brief A comms client.
  protected: std::unique_ptr<CommsClient> client2;
};

/////////////////////////////////////////////////
TEST_F(CommsTest, BadUsage)
{
  this->TestBadUsage();
}

/////////////////////////////////////////////////
TEST_F(CommsTest, Comms)
{
  using namespace std::chrono_literals;

  // Unicast.
  this->client2->SendTo("_data_", this->addr1);

  waitUntilBoolVar(this->unicastCallbackExecuted, 1ms, 300);
  EXPECT_TRUE(this->unicastCallbackExecuted);
  EXPECT_FALSE(this->multicastCallbackExecuted);

  this->Reset();

  // Multicast.
  this->client2->SendTo("_data_", kMulticast);

  waitUntilBoolVar(this->multicastCallbackExecuted, 1ms, 300);
  EXPECT_FALSE(this->unicastCallbackExecuted);
  EXPECT_TRUE(this->multicastCallbackExecuted);

  this->Reset();

  // Broadcast.
  this->client2->SendTo("_data_", kBroadcast);

  waitUntilBoolVar(this->unicastCallbackExecuted, 1ms, 300);
  EXPECT_TRUE(this->unicastCallbackExecuted);
  EXPECT_FALSE(this->multicastCallbackExecuted);
}

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "comms_test");

  return RUN_ALL_TESTS();
}
