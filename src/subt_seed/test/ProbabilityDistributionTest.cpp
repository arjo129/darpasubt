#include <ros/ros.h>
// Bring in gtest
#include <gtest/gtest.h>
#include <subt_seed/ProbabilityDistribution.h>

// Declare a test
TEST(TestSuite, testCase1)
{
//<test things here, calling EXPECT_* and/or ASSERT_* macros as needed>
}

// Declare another test
TEST(TestSuite, testCase2)
{
//<test things here, calling EXPECT_* and/or ASSERT_* macros as needed>
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "tester");
  ros::NodeHandle nh;
  return RUN_ALL_TESTS();
}
