#include <Arduino.h>
#include <params.h>

#define SHAFT_TO_ENCODER_FACTOR 20.4 // Shaft to encoder revolution factor.

typedef enum
{
  WHEEL_POS_TOP_LEFT,
  WHEEL_POS_TOP_RIGHT,
  WHEEL_POS_BOTTOM_LEFT,
  WHEEL_POS_BOTTOM_RIGHT
} WheelPosition_t;

typedef enum
{
  TWIST_OK, // Returned when calculation or set values are valid.
  TWIST_EX_LIM, // Returned when some limits are exceeded.
  TWIST_ZERO // Returned when calculated or set values are zeroed.
} TwistError_t;

typedef struct 
{
  double x;
  double y;
  double z;
} LinearVels_t;

typedef struct 
{
  double x;
  double y;
  double z;
} AngularVels_t;

typedef struct
{
  double length;
  double breadth;
  double diagonal;
  double diagonalHalf;
} PlatformDimensions_t;

typedef struct 
{
  double steerAngle;
  double posAngle;
  double speed;
} DriveParams_t;

typedef struct
{
  WheelPosition_t wheelPos;
  double radius;
  double pivotDist;
} WheelParams_t;

TwistError_t solveTwist(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive);
static double solvBodyRadius(LinearVels_t linear, AngularVels_t angular);
static TwistError_t solvSpotTurn(AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive);
static TwistError_t solvInArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive);
static TwistError_t solvOutArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive);
static TwistError_t solvArcTurn(LinearVels_t linear, AngularVels_t angular, PlatformDimensions_t platform, WheelParams_t wheel, DriveParams_t* drive);