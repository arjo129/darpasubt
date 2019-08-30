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
void setAllMotorsSpeed(long speed);
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
DriveParams_t driveA;
WheelParams_t wheelA;
DriveParams_t driveB;
WheelParams_t wheelB;
DriveParams_t driveC;
WheelParams_t wheelC;
DriveParams_t driveD;
WheelParams_t wheelD;

ros::NodeHandle nh;
std_msgs::Int32 encoderAMsg;
std_msgs::Int32 encoderBMsg;
std_msgs::Int32 encoderCMsg;
std_msgs::Int32 encoderDMsg;
std_msgs::String debugMsg;
ros::Subscriber<geometry_msgs::Twist> twistSub("twist_cmd", &twistCb);
ros::Publisher encoderAPub("encoder_A_speed", &encoderAMsg);
ros::Publisher encoderBPub("encoder_B_speed", &encoderBMsg);
ros::Publisher encoderCPub("encoder_C_speed", &encoderCMsg);
ros::Publisher encoderDPub("encoder_D_speed", &encoderDMsg);
ros::Publisher debugPub("platform_debug", &debugMsg);

uint16_t res;
bool haveTwist = false;

void setup() {
  nh.initNode();
  nh.advertise(encoderAPub);
  nh.advertise(encoderBPub);
  nh.advertise(encoderCPub);
  nh.advertise(encoderDPub);
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
  
  // Publish all encoder data.
  encoderAPub.publish(&encoderAMsg);
  encoderBPub.publish(&encoderBMsg);
  encoderCPub.publish(&encoderCMsg);
  encoderDPub.publish(&encoderDMsg);

  nh.spinOnce();
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
  servoA.write(90);
  servoB.write(90);
  servoC.write(90);
  servoD.write(90);
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
  encoderAMsg.data = scA.correctPwm();
  encoderBMsg.data = scB.correctPwm();
  encoderCMsg.data = scC.correctPwm();
  encoderDMsg.data = scD.correctPwm();
}

/**
 * @brief Sets the speed of all motors.
 * 
 * @param speed The speed in deg/sec.
 */
void setAllMotorsSpeed(long speed)
{
  scA.setSpeed(speed);
  scB.setSpeed(speed);
  scC.setSpeed(speed);
  scD.setSpeed(speed);
}

/**
 * @brief Sets direction of all motors.
 * 
 * @param dir The direction of MOTOR_DIR_t enum.
 */
void setAllMotorsDir(MOTOR_DIR_t dir)
{
  motorA.setDir(dir);
  motorB.setDir(dir);
  motorC.setDir(dir);
  motorD.setDir(dir);
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
  adjust();
}

/**
 * @brief Turn all steering servos given their drive params.
 * 
 */
void turnServos(void)
{
  servoA.write(driveA.posAngle);
  servoB.write(driveB.posAngle);
  servoC.write(driveC.posAngle);
  servoD.write(driveD.posAngle);
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

  wheelA.wheelPos = WHEEL_POS_TOP_LEFT;
  wheelA.radius = WHEEL_RADIUS;
  wheelA.servCalib = SERVO_A_CALIB_VAL;
  driveA.speed = 0;
  driveA.steerAngle = 0;
  driveA.posAngle = 90;
  
  wheelB.wheelPos = WHEEL_POS_TOP_RIGHT;
  wheelB.radius = WHEEL_RADIUS;
  wheelB.servCalib = SERVO_B_CALIB_VAL;
  driveB.speed = 0;
  driveB.steerAngle = 0;
  driveB.posAngle = 90;
  
  wheelC.wheelPos = WHEEL_POS_BOTTOM_LEFT;
  wheelC.radius = WHEEL_RADIUS;
  wheelC.servCalib = SERVO_C_CALIB_VAL;
  driveC.speed = 0;
  driveC.steerAngle = 0;
  driveC.posAngle = 90;
  
  wheelD.wheelPos = WHEEL_POS_BOTTOM_RIGHT;
  wheelD.radius = WHEEL_RADIUS;
  wheelD.servCalib = SERVO_D_CALIB_VAL;
  driveD.speed = 0;
  driveD.steerAngle = 0;
  driveD.posAngle = 90;

  turnServos();
}

/**
 * @brief Rotate all wheels given their drive params.
 * 
 */
void turnWheels(void)
{
  scA.setSpeed(driveA.speed);
  scB.setSpeed(driveB.speed);
  scC.setSpeed(driveC.speed);
  scD.setSpeed(driveD.speed);
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
  if (abs(driveA.speed) > MOTOR_SPEED_LIMIT ||
    abs(driveB.speed) > MOTOR_SPEED_LIMIT ||
    abs(driveC.speed) > MOTOR_SPEED_LIMIT ||
    abs(driveD.speed) > MOTOR_SPEED_LIMIT)
  {
    result |= PARAM_MOTOR_EXCEED;
  }
  
  // Check for servos limit.
  if ((driveB.posAngle - wheelB.servCalib) < SERVO_B_LOWER_LIMIT ||
    (driveB.posAngle - wheelB.servCalib) > SERVO_B_UPPER_LIMIT ||
    (driveD.posAngle - wheelD.servCalib) < SERVO_D_LOWER_LIMIT ||
    (driveD.posAngle - wheelD.servCalib) > SERVO_D_UPPER_LIMIT ||
    (driveA.posAngle - wheelA.servCalib) < SERVO_A_LOWER_LIMIT ||
    (driveA.posAngle - wheelA.servCalib) > SERVO_A_UPPER_LIMIT ||
    (driveC.posAngle - wheelC.servCalib) < SERVO_C_LOWER_LIMIT ||
    (driveC.posAngle - wheelC.servCalib) > SERVO_C_UPPER_LIMIT)
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
  driveA.speed = 0;
  driveB.speed = 0;
  driveC.speed = 0;
  driveD.speed = 0;
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

  driveA.speed = 0;
  driveA.steerAngle = 0;
  driveA.posAngle = 90;
  
  driveB.speed = 0;
  driveB.steerAngle = 0;
  driveB.posAngle = 90;
  
  driveC.speed = 0;
  driveC.steerAngle = 0;
  driveC.posAngle = 90;
  
  driveD.speed = 0;
  driveD.steerAngle = 0;
  driveD.posAngle = 90;
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
  angular.z = twistMsg.angular.z;
}

/**
 * @brief Executes the parameters received from Twist message.
 * 
 */
void handleTwist(void)
{
  TwistError_t errA, errB, errC, errD;
  errA = solveTwist(linear, angular, platform, wheelA, &driveA);
  errB = solveTwist(linear, angular, platform, wheelB, &driveB);
  errC = solveTwist(linear, angular, platform, wheelC, &driveC);
  errD = solveTwist(linear, angular, platform, wheelD, &driveD);
  res = checkParams();
  if (res != PARAM_OK || errA != TWIST_OK || errB != TWIST_OK || errC != TWIST_OK || errD != TWIST_OK)
  {
    handleParamErr(res);
  }
  else
  {
    turnServos();
    turnWheels();

    Serial.println(driveA.speed);
    Serial.println(driveA.steerAngle);
    Serial.println(driveB.speed);
    Serial.println(driveB.steerAngle);
    Serial.println(driveC.speed);
    Serial.println(driveC.steerAngle);
    Serial.println(driveD.speed);
    Serial.println(driveD.steerAngle);
  }
}