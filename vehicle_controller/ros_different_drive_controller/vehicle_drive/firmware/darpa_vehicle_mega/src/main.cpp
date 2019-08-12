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

#include <Arduino.h>
#include <SpeedControl.h>
#include <Servo.h>
#include <main.h>

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

// Function prototypes
void initMotors(void);
void initPwmCorrectTimer(uint32_t microseconds);
void updateA(void);
void updateB(void);
void updateC(void);
void updateD(void);
void adjust(void);
void setAllMotorsSpeed(long speed);
void setAllMotorsDir(MOTOR_DIR_t dir);

void setup() {
  Serial.begin(9600);

  initMotors();
  delay(1500); // Delay 1.5 seconds to allow servos to move to 0 degree position.
  initPwmCorrectTimer(ENCODER_COMM_DELTA_T);
}

void loop() {
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
  motorA.setDir(MOTOR_DIR_FWD);
  motorA.invertDir(true);
  motorA.setPwm(0);
  motorA.start();
  motorB.setDir(MOTOR_DIR_FWD);
  motorB.setPwm(0);
  motorB.start();
  motorC.setDir(MOTOR_DIR_FWD);
  motorC.invertDir(true);
  motorC.setPwm(0);
  motorC.start();
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
  servoA.write(0);
  servoB.write(0);
  servoC.write(0);
  servoD.write(0);
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
  scA.correctPwm();
  scB.correctPwm();
  scC.correctPwm();
  scD.correctPwm();
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