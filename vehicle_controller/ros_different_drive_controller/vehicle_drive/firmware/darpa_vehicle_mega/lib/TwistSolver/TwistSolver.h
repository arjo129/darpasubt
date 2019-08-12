#include <Arduino.h>

typedef enum
{
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT
} WheelPosition_t;

typedef struct 
{
  double x;
  double y;
  double z;
} LinearVels;

typedef struct 
{
  double x;
  double y;
  double z;
} AngularVels;

typedef struct
{
  float length;
  float width;
  float wheelRadius;
} PlatformDimensions;

typedef struct 
{
  double steerAngle;
  double speed;
} DriveParams;



void solveTwist(LinearVels linear, AngularVels angular, PlatformDimensions platform, WheelPosition_t wheel, DriveParams* drive);
double solvTurnRadius(LinearVels linear, AngularVels angular);