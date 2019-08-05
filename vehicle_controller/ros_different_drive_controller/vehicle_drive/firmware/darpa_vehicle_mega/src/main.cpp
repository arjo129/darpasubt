#include <Arduino.h>
#include <SpeedControl.h>
#include <TimerOne.h>
#include <Servo.h>
#include <main.h>

// All global objects.
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
void updateA(void);
void updateB(void);
void updateC(void);
void updateD(void);
void adjust(void);

void setup() {
  Serial.begin(9600);

  // Initialise steering servos.
  servoA.attach(SERVO_A_PWM_PIN);
  servoB.attach(SERVO_B_PWM_PIN);
  servoC.attach(SERVO_C_PWM_PIN);
  servoD.attach(SERVO_D_PWM_PIN);

  // Initialise Timer1 to be used for correcting motor pwm.
  Timer1.initialize(ENCODER_COMM_DELTA_T);
  Timer1.attachInterrupt(adjust);
}

void loop() {

}

/**
 * @brief Initialises the motors, encoders and PIDs object for movement.
 * 
 */
void initMotors(void)
{
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN_A), updateA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN_A), updateB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_C_PIN_A), updateC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_D_PIN_A), updateD, CHANGE);
  motorA.setDir(MOTOR_DIR_FWD);
  motorA.setPwm(0);
  motorA.start();
  motorB.setDir(MOTOR_DIR_FWD);
  motorB.setPwm(0);
  motorB.start();
  motorC.setDir(MOTOR_DIR_FWD);
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
