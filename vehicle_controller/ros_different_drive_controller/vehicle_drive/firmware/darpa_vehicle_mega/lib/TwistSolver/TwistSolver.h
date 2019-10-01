#include <Arduino.h>
#include <params.h>

static const double SHAFT_TO_ENCODER_FACTOR = 98.78; // Shaft to encoder revolution factor.
static const double CONV_TO_ENC_SPD = ((1.0 / (2.0 * M_PI)) * 360.0) * SHAFT_TO_ENCODER_FACTOR;
static const double RADS_TO_DEGS = (1.0 / (2.0 * M_PI)) * 360.0;

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
  TWIST_ZERO, // Returned when calculated or set values are zeroed.
  TWIST_UNKNOWN // Returned when unknown velocities combination detected.
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
  double lengthHalf;
  double breadth;
  double breadthHalf;
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
  double servCalib;
} WheelParams_t;

typedef struct
{
  WheelParams_t wheelParams;
  DriveParams_t driveParams;
} DriveUnit_t;

typedef struct
{
  DriveUnit_t driveUnitA;
  DriveUnit_t driveUnitB;
  DriveUnit_t driveUnitC;
  DriveUnit_t driveUnitD;
} DriveSet_t;


TwistError_t solveTwist(const LinearVels_t& linear, const AngularVels_t& angular, const PlatformDimensions_t& platform, DriveSet_t* driveSet);
static double solvBodyRadius(const LinearVels_t& linear,const AngularVels_t& angular);
static TwistError_t solvSpotTurn(const AngularVels_t& angular, const PlatformDimensions_t& platform, DriveSet_t* driveSet);
static TwistError_t solvArcTurn(const LinearVels_t& linear, const AngularVels_t& angular, const PlatformDimensions_t& platform, DriveSet_t* driveSet);
static TwistError_t solvStrafe(const LinearVels_t& linear, const PlatformDimensions_t& platform, DriveSet_t* driveSet);