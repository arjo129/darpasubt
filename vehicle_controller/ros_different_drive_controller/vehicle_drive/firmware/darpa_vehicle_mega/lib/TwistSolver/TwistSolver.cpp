#include <TwistSolver.h>

void solveTwist(LinearVels linear, AngularVels angular, PlatformDimensions platform, WheelPosition_t wheel, DriveParams* drive)
{
  double bodyTurnRadius = solvTurnRadius(linear, angular);
  
  if (linear.x == 0 && angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = 0;
    return;
  }
  // Forward or backward movement only.
  else if (linear.x != 0 && angular.z == 0)
  {
    drive->speed = ((linear.x / platform.wheelRadius) / (2 * M_PI)) * 360.0;
  }
  // Turning on the spot.
  else if (linear.x == 0 && angular.z != 0)
  {

  }
  // Steering movement.
  else
  {
    
  }
}

/**
 * @brief Compute the turn radius of a body given it's linear and angular velocities.
 * 
 * @param linear LinearVels type representing the body's linear velocity.
 * @param angular AngularVels type representing the body's angular velocity.
 * @return double The corresponding turn radius of the body.
 */
double solvTurnRadius(LinearVels linear, AngularVels angular)
{
  double turnRadius = 0;

  if (linear.x >= 0)
  {
    turnRadius = linear.x / angular.z;
  }
  else
  {
    turnRadius = (-1.0 * linear.x) / angular.z;
  }
  
  return turnRadius;
}