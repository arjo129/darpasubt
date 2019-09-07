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
  if (linear.x == 0 && linear.y == 0 && angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->posAngle = 90 + wheel.servCalib;
    drive->speed = 0;
    return TWIST_OK;
  }
  // Strafing movement only.
  else if (angular.z == 0)
  {
    return solvStrafe(linear, platform, wheel, drive);
  }
  // Turning on the spot.
  else if (linear.x == 0 && linear.y == 0 && angular.z != 0)
  {
    return solvSpotTurn(angular, platform, wheel, drive);
  }
  // Arc movement.
  else if (linear.x != 0 && linear.y == 0 && angular.z != 0)
  {
    return solvArcTurn(linear, angular, platform, wheel, drive);
  }

  return TWIST_UNKNOWN;
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
  return linear.x / angular.z;
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
  double driveSpeed = platform.diagonalHalf * angular.z / wheel.radius;
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
  double steerAngle = asin(platform.breadthHalf / platform.diagonalHalf);
  if (wheel.wheelPos == WHEEL_POS_TOP_LEFT || wheel.wheelPos == WHEEL_POS_BOTTOM_RIGHT) steerAngle *= -1;
  drive->steerAngle = steerAngle;

  // Convert drive speed to degrees/second for the encoder
  drive->speed = ((drive->speed / (2.0 * M_PI)) * 360.0) * SHAFT_TO_ENCODER_FACTOR;

  drive->steerAngle = (drive->steerAngle / (2.0 * M_PI)) * 360.0; // Convert to degrees.
  drive->posAngle = 90 - drive->steerAngle + wheel.servCalib;

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
  WheelPosition_t pos = wheel.wheelPos;
  
  if (angular.z == 0)
  {
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_ZERO;
  }
  
  double bodyRadius = solvBodyRadius(linear, angular);
  if (fabs(bodyRadius) < PLATFORM_RADIUS_LIM)
  {
    // Body radius is too low, there is no valid steer angle.
    drive->steerAngle = 0;
    drive->speed = 0;
    return TWIST_EX_LIM;
  }

  ////////// Compute steering angle
  double steerAngle = 0;
  double arcRadius = 0;
  double vertical = 0;
  switch (pos)
  {
    case WHEEL_POS_TOP_RIGHT:
      arcRadius = bodyRadius + platform.lengthHalf;
      vertical = platform.breadthHalf;
      break;
    case WHEEL_POS_TOP_LEFT:
      arcRadius = bodyRadius - platform.lengthHalf;
      vertical = platform.breadthHalf;
      break;
    case WHEEL_POS_BOTTOM_LEFT:
      arcRadius = bodyRadius - platform.lengthHalf;
      vertical = -platform.breadthHalf;
      break;
    default:
      arcRadius = bodyRadius + platform.lengthHalf;
      vertical = -platform.breadthHalf;
  }
  steerAngle = atan2(vertical, arcRadius);

  // Correct the calculated angle to the directional angle.
  if (steerAngle >= M_PI_2 && steerAngle <= M_PI)
  {
    // In second quadrant, we want the complement angle.
    steerAngle = M_PI - steerAngle;
  }
  else if (steerAngle <= -M_PI_2 && steerAngle >= -M_PI)
  {
    // In third quadrant, we want the complement angle in negative sign.
    steerAngle = -steerAngle - M_PI;
  }
  drive->steerAngle = (steerAngle / (2.0 * M_PI)) * 360.0; // Convert to degrees.
  drive->posAngle = 90 - drive->steerAngle + wheel.servCalib;
  ////////// End

  ////////// Compute drive speed
  drive->speed = arcRadius * angular.z / wheel.radius; // V = R x W
  if (linear.x < 0) drive->speed *= -1;
  // Convert drive speed to degrees/second for the encoder
  drive->speed = ((drive->speed / (2.0 * M_PI)) * 360.0) * SHAFT_TO_ENCODER_FACTOR;
  ////////// End
  
  return TWIST_OK;
}

/**
 * @brief Determines the drive speed and steer angle for strafing movement.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param wheel WheelParams_t type representing the wheel's paramters.
 * @param drive DriveParams_t type representing the computed drive parameters. 
 * @return TwistError_t Error enumeration indicating calculation results. 
 */
static TwistError_t solvStrafe(LinearVels_t linear, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive)
{
  // Get the steer angle first.
  // We use a negative linear.y to follow mathematical convention where positive horizontal axis denotes right.
  double dirAngle = atan2(linear.x, -linear.y);
  double steerAngle = 0;
  if (dirAngle >= 0 && dirAngle <= M_PI)
  {
    steerAngle = dirAngle - M_PI_2;
  }
  else
  {
    steerAngle = dirAngle + M_PI_2;
  }
  drive->steerAngle = (steerAngle / (2.0 * M_PI)) * 360.0; // Convert to degrees.
  drive->posAngle = 90 - drive->steerAngle + wheel.servCalib;

  // Get the drive speed.
  double speedMs = sqrt((linear.x * linear.x) + (linear.y * linear.y));
  drive->speed = speedMs / wheel.radius;
  if (dirAngle < 0) drive->speed *= -1;
  // Convert drive speed to degrees/second for the encoder
  drive->speed = ((drive->speed / (2.0 * M_PI)) * 360.0) * SHAFT_TO_ENCODER_FACTOR;

  return TWIST_OK;
}
