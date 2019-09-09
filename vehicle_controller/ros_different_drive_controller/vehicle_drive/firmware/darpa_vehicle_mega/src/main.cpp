/** 
 * Timers 0 to 5 are assigned in the following manner:
 * Timer 0 - Reserved for Arduino framework
 * Timer 1 - Motor
 * Timer 2 - Motor
 * Timer 3 - Periodic interrupt functions (for PID pwm correction)
 * Timer 4 - Servo
 * Timer 5 - Servo
 * 
 * Motor A - PWM: Pin 11 Dir: 26
 * Motor B - PWM: Pin 12 Dir: 27
 * Motor C - PWM: Pin 13 Dir: 28
 * Motor D - PWM: Pin 10 Dir: 29
 * 
 * Servo A - PWM: Pin 6
 * Servo B - PWM: Pin 7
 * Servo C - PWM: Pin 8
 * Servo D - PWM: Pin 46
 * 
 * The pins are assigned this way to avoid conflicts in usage of Timers.
 * In ServoTimers.h (Servo library), timers 1 and 3 are disabled to prevent conflicts.
 * 
 * TODO:
 * - Fix PID integral value contributing to speed accumulation.
 */

#include <main.h>

// Function prototypes
void initMotors(void);
void initPwmCorrectTimer(uint32_t microseconds);
void initControlParams(void);
void resetDriveParams(void);
void updateA(void);
void updateB(void);
void updateC(void);
void updateD(void);
void adjust(void);
void pubData(void);
void turnServos(void);
void turnWheels(void);
uint16_t checkParams(void);
void handleParamErr(uint16_t err);
void twistCb(const geometry_msgs::Twist& twistMsg);
void handleTwist(void);

// All global variables.
Motor motorA(MOTOR_A_DIR_PIN, MOTOR_A_PWM_PIN);
Motor motorB(MOTOR_B_DIR_PIN, MOTOR_B_PWM_PIN);
Motor motorC(MOTOR_C_DIR_PIN, MOTOR_C_PWM_PIN);
Motor motorD(MOTOR_D_DIR_PIN, MOTOR_D_PWM_PIN);
Encoder encoderA(
  ENCODER_A_PIN_A, 
  ENCODER_A_PIN_B, 
  ENCODER_A_PORT_A, 
  ENCODER_A_PORT_B, 
  ENCODER_COMM_DELTA_T, 
  ENCODER_COMM_TICKS_PER_REV
  );
Encoder encoderB(
  ENCODER_B_PIN_A, 
  ENCODER_B_PIN_B, 
  ENCODER_B_PORT_A, 
  ENCODER_B_PORT_B, 
  ENCODER_COMM_DELTA_T, 
  ENCODER_COMM_TICKS_PER_REV
  );
Encoder encoderC(
  ENCODER_C_PIN_A, 
  ENCODER_C_PIN_B, 
  ENCODER_C_PORT_A, 
  ENCODER_C_PORT_B, 
  ENCODER_COMM_DELTA_T, 
  ENCODER_COMM_TICKS_PER_REV
  );
Encoder encoderD(
  ENCODER_D_PIN_A, 
  ENCODER_D_PIN_B, 
  ENCODER_D_PORT_A, 
  ENCODER_D_PORT_B, 
  ENCODER_COMM_DELTA_T, 
  ENCODER_COMM_TICKS_PER_REV
  );
SpeedControl scA(&motorA, &encoderA);
SpeedControl scB(&motorB, &encoderB);
SpeedControl scC(&motorC, &encoderC);
SpeedControl scD(&motorD, &encoderD);
Servo servoA;
Servo servoB;
Servo servoC;
Servo servoD;

LinearVels_t linear;
AngularVels_t angular;
PlatformDimensions_t platform;
DriveSet_t driveSet;

ros::NodeHandle nh;
std_msgs::Int32 encoderAMsg;
std_msgs::Int32 encoderBMsg;
std_msgs::Int32 encoderCMsg;
std_msgs::Int32 encoderDMsg;
std_msgs::Int32 wheelAMsg;
std_msgs::Int32 wheelBMsg;
std_msgs::Int32 wheelCMsg;
std_msgs::Int32 wheelDMsg;
std_msgs::String debugMsg;
ros::Subscriber<geometry_msgs::Twist> twistSub("twist_cmd", &twistCb);
ros::Publisher encoderAPub("encoder_A_speed", &encoderAMsg);
ros::Publisher encoderBPub("encoder_B_speed", &encoderBMsg);
ros::Publisher encoderCPub("encoder_C_speed", &encoderCMsg);
ros::Publisher encoderDPub("encoder_D_speed", &encoderDMsg);
ros::Publisher wheelAPub("wheel_A_angle", &wheelAMsg);
ros::Publisher wheelBPub("wheel_B_angle", &wheelBMsg);
ros::Publisher wheelCPub("wheel_C_angle", &wheelCMsg);
ros::Publisher wheelDPub("wheel_D_angle", &wheelDMsg);
ros::Publisher debugPub("platform_debug", &debugMsg);

uint16_t res;
bool haveTwist = false;
bool needAdjust = false;
long encoderASpd = 0;
long encoderBSpd = 0;
long encoderCSpd = 0;
long encoderDSpd = 0;

void setup() {
  nh.initNode();
  nh.advertise(encoderAPub);
  nh.advertise(encoderBPub);
  nh.advertise(encoderCPub);
  nh.advertise(encoderDPub);
  nh.advertise(wheelAPub);
  nh.advertise(wheelBPub);
  nh.advertise(wheelCPub);
  nh.advertise(wheelDPub);
  nh.advertise(debugPub);
  nh.subscribe(twistSub);

  initMotors();
  delay(3000); // Delay 3 seconds to allow servos to move to 0 degree position.
  initPwmCorrectTimer(ENCODER_COMM_DELTA_T);
  initControlParams();
}

void loop() {
  // Handle any available Twist messages.
  if (haveTwist)
  {
    haveTwist = false;
    handleTwist();
  }

  // Handle PWM adjustment.
  if (needAdjust)
  {
    needAdjust = false;
    adjust();
    pubData();
    nh.spinOnce();
  }
}

/**
 * @brief Initialises the motors, encoders and PIDs object for movement.
 * 
 */
void initMotors(void)
{
  // Intialises all motor components.
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN_A), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN_A), updateB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_C_PIN_A), updateC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_D_PIN_A), updateD, CHANGE);
  motorA.invertDir(true);
  motorA.setDir(MOTOR_DIR_FWD);
  motorA.setPwm(0);
  motorA.start();
  motorB.invertDir(true);
  motorB.setDir(MOTOR_DIR_FWD);
  motorB.setPwm(0);
  motorB.start();
  motorC.invertDir(true);
  motorC.setDir(MOTOR_DIR_FWD);
  motorC.setPwm(0);
  motorC.start();
  motorD.invertDir(true);
  motorD.setDir(MOTOR_DIR_FWD);
  motorD.setPwm(0);
  motorD.start();
  scA.setGains(SC_A_P_GAIN, SC_A_I_GAIN, SC_A_D_GAIN);
  scA.setMinSpeed(0);
  scB.setGains(SC_B_P_GAIN, SC_B_I_GAIN, SC_B_D_GAIN);
  scB.setMinSpeed(0);
  scC.setGains(SC_C_P_GAIN, SC_C_I_GAIN, SC_C_D_GAIN);
  scC.setMinSpeed(0);
  scD.setGains(SC_D_P_GAIN, SC_D_I_GAIN, SC_D_D_GAIN);
  scD.setMinSpeed(0);

  // Initialise steering servos.
  servoA.attach(SERVO_A_PWM_PIN);
  servoB.attach(SERVO_B_PWM_PIN);
  servoC.attach(SERVO_C_PWM_PIN);
  servoD.attach(SERVO_D_PWM_PIN);
  servoA.write(90 + SERVO_A_CALIB_VAL);
  servoB.write(90 + SERVO_B_CALIB_VAL);
  servoC.write(90 + SERVO_C_CALIB_VAL);
  servoD.write(90 + SERVO_D_CALIB_VAL);
}

/**
 * @brief Callback function to update the ticks count of each encoder during rotation.
 * 
 */
void updateA(void)
{
  encoderA.updateCount();
}

/**
 * @brief Callback function to update the ticks count of each encoder during rotation.
 * 
 */
void updateB(void)
{
  encoderB.updateCount();
}

/**
 * @brief Callback function to update the ticks count of each encoder during rotation.
 * 
 */
void updateC(void)
{
  encoderC.updateCount();
}

/**
 * @brief Callback function to update the ticks count of each encoder during rotation.
 * 
 */
void updateD(void)
{
  encoderD.updateCount();
}

/**
 * @brief Adjusts each motor's PWM according to PID configurations.
 * 
 */
void adjust(void)
{
  encoderASpd = scA.correctPwm();
  encoderBSpd = scB.correctPwm();
  encoderCSpd = scC.correctPwm();
  encoderDSpd = scD.correctPwm();
}

/**
 * @brief Publishes all data to rosserial buffer.
 * 
 */
void pubData(void)
{
  encoderAMsg.data = encoderASpd;
  encoderBMsg.data = encoderBSpd;
  encoderCMsg.data = encoderCSpd;
  encoderDMsg.data = encoderDSpd;
  wheelAMsg.data = driveSet.driveUnitA.driveParams.steerAngle;
  wheelBMsg.data = driveSet.driveUnitB.driveParams.steerAngle;
  wheelCMsg.data = driveSet.driveUnitC.driveParams.steerAngle;
  wheelDMsg.data = driveSet.driveUnitD.driveParams.steerAngle;
  encoderAPub.publish(&encoderAMsg);
  encoderBPub.publish(&encoderBMsg);
  encoderCPub.publish(&encoderCMsg);
  encoderDPub.publish(&encoderDMsg);
  wheelAPub.publish(&wheelAMsg);
  wheelBPub.publish(&wheelBMsg);
  wheelCPub.publish(&wheelCMsg);
  wheelDPub.publish(&wheelDMsg);
}

/**
 * @brief Initialises the Timer used to configure for periodic correction of all motors PWM for PID control.
 * 
 * @param microseconds The amount of time interval in microseconds to correct the motors' PWM.
 */
void initPwmCorrectTimer(uint32_t microseconds)
{
  // We use Timer 3 channel A for this periodic interrupt.
  // Clear registers for proper initialisation.
  TCCR3A = 0;
  TCCR3B = 0;
  TIMSK3 = 0;

  TCCR3B |= (1 << WGM32); // Set CTC mode.
  TCCR3B |= (1 << CS32); // Set clock prescaler to clk_freq/256.
  uint32_t count = ((F_CPU / 1000000) * microseconds) / 256; // We are using prescaler of 256.
  OCR3A = count; // Set the calculated count.
  TIMSK3 |= (1 << OCIE3A); // Enable channel A.
}

/**
 * @brief Interrupt Service Routine function for Timer 3, Channel A.
 * 
 */
ISR(TIMER3_COMPA_vect)
{
  needAdjust = true;
}

/**
 * @brief Turn all steering servos given their drive params.
 * 
 */
void turnServos(void)
{
  servoA.write(driveSet.driveUnitA.driveParams.posAngle);
  servoB.write(driveSet.driveUnitB.driveParams.posAngle);
  servoC.write(driveSet.driveUnitC.driveParams.posAngle);
  servoD.write(driveSet.driveUnitD.driveParams.posAngle);
}

/**
 * @brief Initialises all parameters for wheels control.
 * 
 */
void initControlParams(void)
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

  turnServos();
}

/**
 * @brief Rotate all wheels given their drive params.
 * 
 */
void turnWheels(void)
{
  scA.setSpeed(driveSet.driveUnitA.driveParams.speed);
  scB.setSpeed(driveSet.driveUnitB.driveParams.speed);
  scC.setSpeed(driveSet.driveUnitC.driveParams.speed);
  scD.setSpeed(driveSet.driveUnitD.driveParams.speed);
}

/**
 * @brief Check all drive parameters are within limits.
 * 
 * @return uint16_t Errors indicated by bit index position. See CHK_PARAM_RES_t enumeration.
 */
uint16_t checkParams(void)
{
  uint16_t result = PARAM_OK;

  // Check for motors limit.
  if (fabs(driveSet.driveUnitA.driveParams.speed) > MOTOR_SPEED_LIMIT ||
    fabs(driveSet.driveUnitB.driveParams.speed) > MOTOR_SPEED_LIMIT ||
    fabs(driveSet.driveUnitC.driveParams.speed) > MOTOR_SPEED_LIMIT ||
    fabs(driveSet.driveUnitD.driveParams.speed) > MOTOR_SPEED_LIMIT)
  {
    result |= PARAM_MOTOR_EXCEED;
  }
  
  // Check for servos limit.
  if ((driveSet.driveUnitB.driveParams.posAngle - driveSet.driveUnitB.wheelParams.servCalib) < SERVO_B_LOWER_LIMIT ||
    (driveSet.driveUnitB.driveParams.posAngle - driveSet.driveUnitB.wheelParams.servCalib) > SERVO_B_UPPER_LIMIT ||
    (driveSet.driveUnitD.driveParams.posAngle - driveSet.driveUnitD.wheelParams.servCalib) < SERVO_D_LOWER_LIMIT ||
    (driveSet.driveUnitD.driveParams.posAngle - driveSet.driveUnitD.wheelParams.servCalib) > SERVO_D_UPPER_LIMIT ||
    (driveSet.driveUnitA.driveParams.posAngle - driveSet.driveUnitA.wheelParams.servCalib) < SERVO_A_LOWER_LIMIT ||
    (driveSet.driveUnitA.driveParams.posAngle - driveSet.driveUnitA.wheelParams.servCalib) > SERVO_A_UPPER_LIMIT ||
    (driveSet.driveUnitC.driveParams.posAngle - driveSet.driveUnitC.wheelParams.servCalib) < SERVO_C_LOWER_LIMIT ||
    (driveSet.driveUnitC.driveParams.posAngle - driveSet.driveUnitC.wheelParams.servCalib) > SERVO_C_UPPER_LIMIT)
  {
    result |= PARAM_SERVO_EXCEED;
  }

  return result;
}

/**
 * @brief Handle drive parameters out of limits.
 * 
 * @param err Errors indicated by bit index position. See CHK_PARAM_RES_t enumeration.
 */
void handleParamErr(uint16_t err)
{
  driveSet.driveUnitA.driveParams.speed = 0;
  driveSet.driveUnitB.driveParams.speed = 0;
  driveSet.driveUnitC.driveParams.speed = 0;
  driveSet.driveUnitD.driveParams.speed = 0;
  turnWheels();
  delay(1500);
}

/**
 * @brief Resets drive parameters of all wheels to default state.
 * 
 */
void resetDriveParams(void)
{
  linear.x = 0;
  linear.y = 0;
  linear.z = 0;
  angular.x = 0;
  angular.y = 0;
  angular.z = 0;

  driveSet.driveUnitA.driveParams.speed = 0;
  driveSet.driveUnitA.driveParams.steerAngle = 0;
  driveSet.driveUnitA.driveParams.posAngle = 90 + driveSet.driveUnitA.wheelParams.servCalib;
  
  driveSet.driveUnitB.driveParams.speed = 0;
  driveSet.driveUnitB.driveParams.steerAngle = 0;
  driveSet.driveUnitB.driveParams.posAngle = 90 + driveSet.driveUnitB.wheelParams.servCalib;
  
  driveSet.driveUnitC.driveParams.speed = 0;
  driveSet.driveUnitC.driveParams.steerAngle = 0;
  driveSet.driveUnitC.driveParams.posAngle = 90 + driveSet.driveUnitC.wheelParams.servCalib;
  
  driveSet.driveUnitD.driveParams.speed = 0;
  driveSet.driveUnitD.driveParams.steerAngle = 0;
  driveSet.driveUnitD.driveParams.posAngle = 90 + driveSet.driveUnitD.wheelParams.servCalib;
}

/**
 * @brief Callback function for receiving Twist messages.
 * 
 * @param twistMsg Twist message received.
 */
void twistCb(const geometry_msgs::Twist& twistMsg)
{
  haveTwist = true;
  linear.x = twistMsg.linear.x;
  linear.y = twistMsg.linear.y;
  angular.z = twistMsg.angular.z;
}

/**
 * @brief Executes the parameters received from Twist message.
 * 
 */
void handleTwist(void)
{
  TwistError_t err;
  err = solveTwist(linear, angular, platform, &driveSet);
  // res = checkParams();
  if (res != PARAM_OK || err != TWIST_OK)
  {
    handleParamErr(res);
  }
  else
  {
    turnServos();
    turnWheels();
  }
}