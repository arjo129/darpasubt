#include <ros/ros.h>
// Bring in gtest
#include <gtest/gtest.h>
#include <subt_seed/ProbabilityDistribution.h>

// Declare a test
TEST(ProbabilityDistributionTest, equalityTestCheckNaive)
{
    //NULL case
    ProbabilityDistribution prob1, prob2;
    ASSERT_TRUE(prob1 == prob2);

    //Check if different length return false
    prob1.updateProbability(Eigen::Vector3f(0,0,0), 0.1);
    ASSERT_TRUE(prob1 != prob2);

    //Check if different values return false
    prob2.updateProbability(Eigen::Vector3f(0,0,0), 0.0);
    ASSERT_TRUE(prob1 != prob2);

    //Check if same values return true
    prob2.updateProbability(Eigen::Vector3f(0,0,0), 0.1);
    ASSERT_TRUE(prob1 == prob2);
}

// Declare another test
TEST(ProbabilityDistributionTest, multiplicationTest)
{
    ProbabilityDistribution prob1, prob2, expected;

    //Same size, all overlap
    prob1.updateProbability(Eigen::Vector3f(0,0,1), 0.1);
    prob2.updateProbability(Eigen::Vector3f(0,0,1), 0.1);
    expected.updateProbability(Eigen::Vector3f(0,0,1), 0.01);
    ASSERT_DOUBLE_EQ((prob1 && prob2).get(Eigen::Vector3f(0,0,1)), 0.01);
    ASSERT_TRUE((prob1 && prob2) == expected);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  //ros::init(argc, argv, "tester");
  //ros::NodeHandle nh;
  return RUN_ALL_TESTS();
}
