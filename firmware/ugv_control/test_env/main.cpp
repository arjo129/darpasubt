#include <iostream>
#include <TwistSolver.h>

void initParams(void);

LinearVels_t linear;
AngularVels_t angular;
PlatformDimensions_t platform;
DriveSet_t driveSet;

int main(void)
{
  
  initParams();
  
  solveTwist(linear, angular, platform, &driveSet); 
  cout << driveSet.driveUnitA.driveParams << endl;

  return 0;
}

void initParams(void)
{
  platform.length = PLATFORM_LENGTH;
  platform.lengthHalf = PLATFORM_LENGTH * 0.5;
  platform.breadth = PLATFORM_BREADTH;
  platform.breadthHalf = PLATFORM_BREADTH * 0.5;
  platform.diagonal = sqrt((PLATFORM_LENGTH * PLATFORM_LENGTH) + (PLATFORM_BREADTH * PLATFORM_BREADTH));
  platform.diagonalHalf = platform.diagonal / 2.0;
  
  linear.x = 0;
  linear.y = 0;
  linear.z = 0;
  angular.x = 0;
  angular.y = 0;
  angular.z = 0;

  driveSet.driveUnitA.wheelParams.wheelPos = WHEEL_POS_TOP_LEFT;
  driveSet.driveUnitA.wheelParams.radius = WHEEL_RADIUS;
  driveSet.driveUnitA.wheelParams.servCalib = SERVO_A_CALIB_VAL;
  driveSet.driveUnitA.driveParams.speed = 0;
  driveSet.driveUnitA.driveParams.steerAngle = 0;
  driveSet.driveUnitA.driveParams.posAngle = 90 + SERVO_A_CALIB_VAL;
  
  driveSet.driveUnitB.wheelParams.wheelPos = WHEEL_POS_TOP_RIGHT;
  driveSet.driveUnitB.wheelParams.radius = WHEEL_RADIUS;
  driveSet.driveUnitB.wheelParams.servCalib = SERVO_B_CALIB_VAL;
  driveSet.driveUnitB.driveParams.speed = 0;
  driveSet.driveUnitB.driveParams.steerAngle = 0;
  driveSet.driveUnitB.driveParams.posAngle = 90 + SERVO_B_CALIB_VAL;
  
  driveSet.driveUnitC.wheelParams.wheelPos = WHEEL_POS_BOTTOM_LEFT;
  driveSet.driveUnitC.wheelParams.radius = WHEEL_RADIUS;
  driveSet.driveUnitC.wheelParams.servCalib = SERVO_C_CALIB_VAL;
  driveSet.driveUnitC.driveParams.speed = 0;
  driveSet.driveUnitC.driveParams.steerAngle = 0;
  driveSet.driveUnitC.driveParams.posAngle = 90 + SERVO_C_CALIB_VAL;
  
  driveSet.driveUnitD.wheelParams.wheelPos = WHEEL_POS_BOTTOM_RIGHT;
  driveSet.driveUnitD.wheelParams.radius = WHEEL_RADIUS;
  driveSet.driveUnitD.wheelParams.servCalib = SERVO_D_CALIB_VAL;
  driveSet.driveUnitD.driveParams.speed = 0;
  driveSet.driveUnitD.driveParams.steerAngle = 0;
  driveSet.driveUnitD.driveParams.posAngle = 90 + SERVO_D_CALIB_VAL;
}
