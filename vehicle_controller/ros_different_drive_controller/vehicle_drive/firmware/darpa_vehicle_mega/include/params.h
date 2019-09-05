// Platform and wheel dimensions.
#define PLATFORM_LENGTH 0.414 // Wheel pivot point to wheel pivot point.
#define PLATFORM_BREADTH 0.414 // Wheel pivot point to wheel pivot point.
#define PLATFORM_RADIUS_BUF 0
// A body radius limit is defined here so the calculated speed and steer angles are within limits.
#define PLATFORM_RADIUS_LIM ((0.5 * PLATFORM_LENGTH) + PLATFORM_RADIUS_BUF)
#define WHEEL_RADIUS 0.06
#define WHEEL_PIVOT_DIST 0.08 // Distance between wheel-ground contact point and pivot point.

// Servos and motor limits.
#define SERVO_OUTER_LIMIT 0.0
#define SERVO_INNER_LIMIT 0.0
#define SERVO_A_CALIB_VAL -2 // Calibration values are the positive/negative offset from 90 degrees position.
#define SERVO_B_CALIB_VAL -5
#define SERVO_C_CALIB_VAL -5
#define SERVO_D_CALIB_VAL 7
#define SERVO_A_UPPER_LIMIT 180.0 - SERVO_OUTER_LIMIT // Minimum degree position for outer rotation.
#define SERVO_A_LOWER_LIMIT SERVO_INNER_LIMIT // Maximum degree position for inner rotation. 
#define SERVO_B_UPPER_LIMIT 180.0 - SERVO_INNER_LIMIT // Minimum degree position for outer rotation.
#define SERVO_B_LOWER_LIMIT SERVO_OUTER_LIMIT // Minimum degree position for outer rotation.
#define SERVO_C_UPPER_LIMIT 180.0 - SERVO_INNER_LIMIT // Maximum degree position for inner rotation. 
#define SERVO_C_LOWER_LIMIT SERVO_OUTER_LIMIT // Maximum degree position for inner rotation. 
#define SERVO_D_UPPER_LIMIT 180.0 - SERVO_OUTER_LIMIT // Maximum degree position for inner rotation. 
#define SERVO_D_LOWER_LIMIT SERVO_INNER_LIMIT // Maximum degree position for inner rotation. 
#define MOTOR_SPEED_LIMIT 40000 // In degress per second of the encoder.