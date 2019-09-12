#include <TwistSolver.h>

/**
 * @brief Solves a wheel's steering angle and drive speed given the linear and angular velocities
 * from a Twist message.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param driveSet A set of DriveUnit_t representing every wheels' and motors' parameters.
 * return TwistError_t Error enumeration indicating calculation results.
 */
TwistError_t solveTwist(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, DriveSet_t* driveSet)
{
  if (linear.x == 0 && linear.y == 0 && angular.z == 0)
  {
    driveSet->driveUnitA.driveParams.steerAngle = 0;
    driveSet->driveUnitA.driveParams.posAngle = 90 + driveSet->driveUnitA.wheelParams.servCalib;
    driveSet->driveUnitA.driveParams.speed = 0;
    
    driveSet->driveUnitB.driveParams.steerAngle = 0;
    driveSet->driveUnitB.driveParams.posAngle = 90 + driveSet->driveUnitB.wheelParams.servCalib;
    driveSet->driveUnitB.driveParams.speed = 0;

    driveSet->driveUnitC.driveParams.steerAngle = 0;
    driveSet->driveUnitC.driveParams.posAngle = 90 + driveSet->driveUnitC.wheelParams.servCalib;
    driveSet->driveUnitC.driveParams.speed = 0;

    driveSet->driveUnitD.driveParams.steerAngle = 0;
    driveSet->driveUnitD.driveParams.posAngle = 90 + driveSet->driveUnitD.wheelParams.servCalib;
    driveSet->driveUnitD.driveParams.speed = 0;
    return TWIST_OK;
  }
  // Strafing movement only.
  else if (angular.z == 0)
  {
    return solvStrafe(linear, platform, driveSet);
  }
  // Turning on the spot.
  else if (linear.x == 0 && linear.y == 0 && angular.z != 0)
  {
    return solvSpotTurn(angular, platform, driveSet);
  }
  // Arc movement.
  else if (linear.x != 0 && linear.y == 0 && angular.z != 0)
  {
    return solvArcTurn(linear, angular, platform, driveSet);
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
 * @param driveSet A set of DriveUnit_t representing every wheels' and motors' parameters.
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvSpotTurn(AngularVels_t angular, PlatformDimensions_t platform, DriveSet_t* driveSet)
{
  if (angular.z == 0)
  {
    return TWIST_ZERO;
  }

  // Compute the rotation speed required.
  // Since the wheel radius are common to all wheels, we can use one value to calculate for all wheel.
  double driveSpeed = platform.diagonalHalf * angular.z / driveSet->driveUnitA.wheelParams.radius;
  driveSpeed *= CONV_TO_ENC_SPD; // Convert drive speed to degrees/second for the encoder.
  /*
   * Depending if angular.z is positive or negative, computed driveSpeed will be the same sign.
   * 
   * If angular.z is positive, body is spot turning to the left.
   * The left wheels must have driveSpeed of opposite sign.
   * 
   * If angular.z is negative, body is spot turning to the right.
   * The left wheel must still have driveSpeed of opposite sign.
   */
  driveSet->driveUnitA.driveParams.speed = -driveSpeed;
  driveSet->driveUnitB.driveParams.speed = driveSpeed;
  driveSet->driveUnitC.driveParams.speed = -driveSpeed;
  driveSet->driveUnitD.driveParams.speed = driveSpeed;
  
  // Compute the steer angle required.
  double steerAngle = asin(platform.breadthHalf / platform.diagonalHalf);
  steerAngle *= RADS_TO_DEGS; // Convert to degrees.
  driveSet->driveUnitA.driveParams.steerAngle = -steerAngle;
  driveSet->driveUnitB.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitC.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitD.driveParams.steerAngle = -steerAngle;

  driveSet->driveUnitA.driveParams.posAngle = 90 + steerAngle + driveSet->driveUnitA.wheelParams.servCalib;
  driveSet->driveUnitB.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitB.wheelParams.servCalib;
  driveSet->driveUnitC.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitC.wheelParams.servCalib;
  driveSet->driveUnitD.driveParams.posAngle = 90 + steerAngle + driveSet->driveUnitD.wheelParams.servCalib;

  return TWIST_OK;
}

/**
 * @brief Determines how to compute the steering angle and drive speed of a single wheel during arc
 * turning, given the body's linear and angular velocities.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param angular AngularVels_t type representing the body's angular velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param driveSet A set of DriveUnit_t representing every wheels' and motors' parameters. 
 * @return TwistError_t Error enumeration indicating calculation results.
 */
static TwistError_t solvArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, DriveSet_t *driveSet)
{
  if (angular.z == 0)
  {
    return TWIST_ZERO;
  }
  
  double bodyRadius = solvBodyRadius(linear, angular);
  if (fabs(bodyRadius) < PLATFORM_RADIUS_LIM)
  {
    // Body radius is too low, there is no valid steer angle.
    return TWIST_EX_LIM;
  }

  ////////// Compute steering angle
  double nearArcRadius, farArcRadius = 0;
  double nearAngle, farAngle = 0;
  if (bodyRadius >= 0)
  {
    nearArcRadius = bodyRadius - platform.lengthHalf;
    farArcRadius = bodyRadius + platform.lengthHalf;
  }
  else
  {
    nearArcRadius = bodyRadius + platform.lengthHalf;
    farArcRadius = bodyRadius - platform.lengthHalf;
  }
  nearAngle = atan2(platform.breadthHalf,nearArcRadius);
  farAngle = atan2(platform.breadthHalf, farArcRadius);
  
  // In second quadrant, we want the complement angle.
  if (nearAngle >= M_PI_2 && nearAngle <= M_PI) nearAngle = M_PI - nearAngle;
  if (farAngle >= M_PI_2 && nearAngle <= M_PI) farAngle = M_PI - farAngle;
  
  // Convert the angles from radians to degrees.
  nearAngle *= RADS_TO_DEGS;
  farAngle *= RADS_TO_DEGS;
  ////////// Compute steering angle end

  ////////// Compute drive speed
  // Since all the wheels have the same radius, we can use one value for every wheels.
  double nearDriveSpeed = nearArcRadius * angular.z / driveSet->driveUnitA.wheelParams.radius;
  double farDriveSpeed = farArcRadius * angular.z / driveSet->driveUnitA.wheelParams.radius;

  // Convert drive speed to degrees/second for the encoder
  nearDriveSpeed = nearDriveSpeed * CONV_TO_ENC_SPD;
  farDriveSpeed = farDriveSpeed * CONV_TO_ENC_SPD;
  ////////// Compute drive speed end

  // We assign the wheels with their respective params.
  // Note: The back wheels would just have the same angle as the wheel in front of them but in opposite 
  if (bodyRadius >= 0)
  {
    driveSet->driveUnitA.driveParams.steerAngle = nearAngle;
    driveSet->driveUnitB.driveParams.steerAngle = farAngle;
    driveSet->driveUnitC.driveParams.steerAngle = -nearAngle;
    driveSet->driveUnitD.driveParams.steerAngle = -farAngle;

    driveSet->driveUnitA.driveParams.posAngle = 90 - nearAngle + driveSet->driveUnitA.wheelParams.servCalib;
    driveSet->driveUnitB.driveParams.posAngle = 90 - farAngle + driveSet->driveUnitB.wheelParams.servCalib;
    driveSet->driveUnitC.driveParams.posAngle = 90 + nearAngle + driveSet->driveUnitC.wheelParams.servCalib;
    driveSet->driveUnitD.driveParams.posAngle = 90 + farAngle + driveSet->driveUnitD.wheelParams.servCalib;

    driveSet->driveUnitA.driveParams.speed = nearDriveSpeed;
    driveSet->driveUnitB.driveParams.speed = farDriveSpeed;
    driveSet->driveUnitC.driveParams.speed = nearDriveSpeed;
    driveSet->driveUnitD.driveParams.speed = farDriveSpeed;
  }
  else
  {
    driveSet->driveUnitA.driveParams.steerAngle = -farAngle;
    driveSet->driveUnitB.driveParams.steerAngle = -nearAngle;
    driveSet->driveUnitC.driveParams.steerAngle = farAngle;
    driveSet->driveUnitD.driveParams.steerAngle = nearAngle;

    driveSet->driveUnitA.driveParams.posAngle = 90 + farAngle + driveSet->driveUnitA.wheelParams.servCalib;
    driveSet->driveUnitB.driveParams.posAngle = 90 + nearAngle + driveSet->driveUnitB.wheelParams.servCalib;
    driveSet->driveUnitC.driveParams.posAngle = 90 - farAngle + driveSet->driveUnitC.wheelParams.servCalib;
    driveSet->driveUnitD.driveParams.posAngle = 90 - nearAngle + driveSet->driveUnitD.wheelParams.servCalib;

    driveSet->driveUnitA.driveParams.speed = farDriveSpeed;
    driveSet->driveUnitB.driveParams.speed = nearDriveSpeed;
    driveSet->driveUnitC.driveParams.speed = farDriveSpeed;
    driveSet->driveUnitD.driveParams.speed = nearDriveSpeed;
  }
  
  return TWIST_OK;
}

/**
 * @brief Determines the drive speed and steer angle for strafing movement.
 * 
 * @param linear LinearVels_t type representing the body's linear velocity.
 * @param platform PlatformDimensions_t type representing the body's dimension parameters.
 * @param driveSet A set of DriveUnit_t representing every wheels' and motors' parameters.
 * @return TwistError_t Error enumeration indicating calculation results. 
 */
static TwistError_t solvStrafe(LinearVels_t linear, PlatformDimensions_t platform, DriveSet_t* driveSet)
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
  steerAngle *= RADS_TO_DEGS; // Convert to degrees.
  driveSet->driveUnitA.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitB.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitC.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitD.driveParams.steerAngle = steerAngle;
  driveSet->driveUnitA.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitA.wheelParams.servCalib;
  driveSet->driveUnitB.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitB.wheelParams.servCalib;
  driveSet->driveUnitC.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitC.wheelParams.servCalib;
  driveSet->driveUnitD.driveParams.posAngle = 90 - steerAngle + driveSet->driveUnitD.wheelParams.servCalib;

  // Get the drive speed.
  // Since all the wheels have the same radius, we can use one value for every wheels.
  double driveSpeed = sqrt((linear.x * linear.x) + (linear.y * linear.y)) / driveSet->driveUnitA.wheelParams.radius;
  driveSpeed *= CONV_TO_ENC_SPD; // Convert drive speed to degrees/second for the encoder
  if (dirAngle < 0) driveSpeed = -driveSpeed;
  driveSet->driveUnitA.driveParams.speed = driveSpeed;
  driveSet->driveUnitB.driveParams.speed = driveSpeed;
  driveSet->driveUnitC.driveParams.speed = driveSpeed;
  driveSet->driveUnitD.driveParams.speed = driveSpeed;

  return TWIST_OK;
}
