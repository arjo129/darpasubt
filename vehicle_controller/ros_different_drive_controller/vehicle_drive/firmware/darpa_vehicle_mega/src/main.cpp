#include <Arduino.h>
#include <SpeedControl.h>
#include <TimerOne.h>
#include <Servo.h>

// All Motors related
#define MOTOR_A_DIR_PIN 22
#define MOTOR_A_PWM_PIN 8
#define ENCODER_A_PIN_A 2
#define ENCODER_A_PIN_B 24
#define ENCODER_A_DELTA_T 50000
#define ENCODER_A_TICKS_PER_REV 24
#define SC_A_P_GAIN 0.00423
#define SC_A_I_GAIN 0.0008
#define SC_A_D_GAIN 0.0005

// All Servos related
#define SERVO_A_PWM_PIN 6

Motor motorA(MOTOR_A_DIR_PIN, MOTOR_A_PWM_PIN);
Encoder encoderA(ENCODER_A_PIN_A, ENCODER_A_PIN_B, ENCODER_A_DELTA_T, ENCODER_A_TICKS_PER_REV);
SpeedControl scA(&motorA, &encoderA);

Servo servo;

// Function prototypes
void update(void);
void adjust(void);

void setup() {
  Serial.begin(9600);

  // Initialise motors.
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN_A), update, CHANGE);;
  motorA.setDir(MOTOR_DIR_FWD);
  motorA.setPwm(0);
  motorA.start();
  scA.setGains(SC_A_P_GAIN, SC_A_I_GAIN, SC_A_D_GAIN);
  scA.setMinSpeed(0);
  scA.setSpeed(15000);

  // Initialise Timer1 to be used for correcting motor pwm.
  Timer1.initialize(ENCODER_A_DELTA_T);
  Timer1.attachInterrupt(adjust);

  // Initialise steering servos.
  servo.attach(SERVO_A_PWM_PIN);
}

void loop() {
  servo.write(0);
  delay(1000);
  servo.write(45);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(135);
  delay(1000);
  servo.write(180);
  delay(1000);
  servo.write(135);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(45);
  delay(1000);
}

/**
 * @brief Callback function to update the ticks count of each encoder during rotation.
 * 
 */
void update(void)
{
  encoderA.updateCount();
}

/**
 * @brief Adjusts each motor's PWM according to PID configurations.
 * 
 */
void adjust(void)
{
  long speed = scA.correctPwm();
}
