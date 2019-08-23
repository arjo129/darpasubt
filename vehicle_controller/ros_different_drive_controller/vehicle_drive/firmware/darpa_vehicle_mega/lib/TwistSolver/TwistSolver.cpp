#include <TwistSolver.h>

/**
 * @brief Solves a wheel's steering angle and drive speed given the linear and angular velocities
 * from a Twist message.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters.
 * return TwistError_t Error enumeration indicating calculation results.
 */
TwistError_t solveTwist(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  TwistError_t res;

  if (linear.x == 0 && angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->posAngle = 90;
    drive->speed = 0;
    return TWIST_OK;
  }
  // Forward or backward movement only.
  else if (linear.x != 0 && angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = linear.x / wheel.radius;
  }
  // Turning on the spot.
  else if (linear.x == 0 && angular.z != 0)
  {
    res = solvSpotTurn(angular, platform, wheel, drive);
  }
  // Steering movement.
  else
  {
    res = solvArcTurn(linear, angular, platform, wheel, drive);
  }

  drive->speed = (drive->speed / (2.0 * M_PI)) * 360.0; // Convert to degrees per second.
  drive->speed *= SHAFT_TO_ENCODER_FACTOR; // We want to get encoder revolution speed.

  drive->steerAngle = (drive->steerAngle / (2.0 * M_PI)) * 360.0; // Convert to degrees.
  drive->posAngle = 90 - drive->steerAngle;

  return res;
}

/**
 * @brief Computes the turn radius of a body given it's linear and angular velocities.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @return double The corresponding turn radius of the body.
 */
 static double solvBodyRadius(LinearVels_t linear, AngularVels_t angular)
{
  double turnRadius = linear.x / angular.z;
  if (turnRadius < 0) turnRadius *= -1;

  return turnRadius;
}

/**
 * @brief Computes the steering angle and drive speed of a single wheel during spot turning, given
 * the body's angular velocities.
 * 
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters.
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvSpotTurn(AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  if (angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_ZERO;
  }

  // Compute the rotation speed required.
  double driveSpeed = (platform.diagonalHalf + wheel.pivotDist) * angular.z;
  /*
   * Depending if angular.z is positive or negative, computed driveSpeed will be the same sign.
   * 
   * If angular.z is positive, body is spot turning to the left.
   * The left wheels must have driveSpeed of opposite sign.
   * 
   * If angular.z is negative, body is spot turning to the right.
   * The left wheel must still have driveSpeed of opposite sign.
   */
  drive->speed = driveSpeed;
  if (wheel.wheelPos == WHEEL_POS_TOP_LEFT || wheel.wheelPos == WHEEL_POS_BOTTOM_LEFT) drive->speed *= -1;
  
  // Compute the steer angle required.
  double steerAngle = atan(platform.length / platform.breadth);
  if (wheel.wheelPos == WHEEL_POS_TOP_LEFT || wheel.wheelPos == WHEEL_POS_BOTTOM_RIGHT)
  {
    steerAngle *= -1;
  }
  drive->steerAngle = steerAngle;

  return TWIST_OK;
}

/**
 * @brief Computes the steering angle and drive speed of wheels from inner arc, given the body's
 * linear and angular velocities.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters. 
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvInArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  if (angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_ZERO;
  }

  // Compute the steering angle required.
  double bodyRadius = solvBodyRadius(linear, angular);
  if (bodyRadius < PLATFORM_RADIUS_LIM)
  {
    // Body radius is too low, there is no valid steer angle.
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_EX_LIM;
  }
  double steerAngle = atan(platform.breadth / ((2.0 * bodyRadius) - platform.length));
  // Arc turn counter clockwise.
  if (angular.z > 0)
  {
    if (wheel.wheelPos == WHEEL_POS_BOTTOM_LEFT) steerAngle *= -1;
  }
  // Arc turn clockwise.
  else if (angular.z < 0)
  {
    if (wheel.wheelPos == WHEEL_POS_TOP_RIGHT) steerAngle *= -1;
  }
  drive->steerAngle = steerAngle;

  // Compute the rotation speed required.
  // Keep all values positive for easier sign manupulation.
  if (angular.z < 0) angular.z *= -1;
  if (steerAngle < 0) steerAngle *= -1;

  double driveSpeed = (angular.z / (2.0 * sin(steerAngle) * wheel.radius)) * (platform.breadth - (wheel.pivotDist * 2.0 * sin(steerAngle)));
  if (linear.x < 0) driveSpeed *= -1; // Movement is backwards.
  drive->speed = driveSpeed;

  return TWIST_OK;
}

/**
 * @brief Computes the steering angle and drive speed of wheels from outer arc, given the body's
 * linear and angular velocities.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters. 
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvOutArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  if (angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_ZERO;
  }

  // Compute the steer angle.
  double bodyRadius = solvBodyRadius(linear, angular);
  if (bodyRadius < PLATFORM_RADIUS_LIM)
  {
    // Body radius is too low, there is no valid steer angle.
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_EX_LIM;
  }
  double steerAngle = M_PI_2 - atan((bodyRadius + (0.5 * platform.length)) / (0.5 * platform.breadth));
  // Arc turn counter clockwise.
  if (angular.z > 0)
  {
    if (wheel.wheelPos == WHEEL_POS_BOTTOM_RIGHT) steerAngle *= -1;
  }
  // Arc turn clockwise.
  else
  {
    if (wheel.wheelPos == WHEEL_POS_TOP_LEFT) steerAngle *= -1;
  }
  drive->steerAngle = steerAngle;

  // Compute the rotation speed required.
  // Keep all values positive for easier sign manupulation.
  if (angular.z < 0) angular.z *= -1;
  if (steerAngle < 0) steerAngle *= -1;

  double driveSpeed = (angular.z / (2.0 * sin(steerAngle) * wheel.radius)) * (platform.breadth + (wheel.pivotDist * 2.0 * sin(steerAngle)));
  if (linear.x < 0) driveSpeed *= -1; // Movement is backwards.
  drive->speed = driveSpeed;

  return TWIST_OK;
}

/**
 * @brief Determines how to compute the steering angle and drive speed of a single wheel during arc
 * turning, given the body's linear and angular velocities.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters. 
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  WheelPosition_t wheelPos = wheel.wheelPos;
  if (angular.z > 0 && (wheelPos == WHEEL_POS_TOP_LEFT || wheelPos == WHEEL_POS_BOTTOM_LEFT))
  {
    if (linear.x > 0)
    {
      return solvInArcTurn(linear, angular, platform, wheel, drive);
    }
    else
    {
      return solvOutArcTurn(linear, angular, platform, wheel, drive);
    }
  }
  else if (angular.z > 0 && (wheelPos == WHEEL_POS_TOP_RIGHT || wheelPos == WHEEL_POS_BOTTOM_RIGHT))
  {
    if (linear.x > 0)
    {
      return solvOutArcTurn(linear, angular, platform, wheel, drive);
    }
    else
    {
      return solvInArcTurn(linear, angular, platform, wheel, drive);
    }
    
  }
  else if (angular.z < 0 && (wheelPos == WHEEL_POS_TOP_RIGHT || wheelPos == WHEEL_POS_BOTTOM_RIGHT))
  {
    if (linear.x > 0)
    {
      return solvInArcTurn(linear, angular, platform, wheel, drive);
    }
    else
    {
      return solvOutArcTurn(linear, angular, platform, wheel, drive);
    }
    
  }
  else
  {
    if (linear.x > 0)
    {
      return solvOutArcTurn(linear, angular, platform, wheel, drive);
    }
    else
    {
      return solvInArcTurn(linear, angular, platform, wheel, drive);
    }
  }
}