#include <Arduino.h>
#include <SpeedControl.h>
#include <TimerOne.h>
#include <Servo.h>
#include <RH_RF95.h>
#include <DataPackage.h>
#include <main.h>

// All global variables.
long currSpeed = 0;
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
SoftwareSerial ss(2, 3);
RH_RF95 radioRecv(ss);
uint8_t bufLen = DataPackage::DATA_LEN;
uint8_t buf[DataPackage::DATA_LEN];
DataPackage::Packet packet;

// Function prototypes
void initMotors(void);
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
  
  // Initialise Timer1 to be used for correcting motor pwm.
  Timer1.initialize(ENCODER_COMM_DELTA_T);
  Timer1.attachInterrupt(adjust);

  // Initialise radio receiver
  if (!radioRecv.init())
  {
    Serial.println("Radio initialisation failed. Check pins again.");
  }
  else
  {
    radioRecv.setFrequency(434.0);
  }
}

void loop() {
  if (radioRecv.available())
  {
    // Read incoming data first.
    radioRecv.recv(buf, &bufLen);
    packet = DataPackage::deserialiseData(buf);

    // Forward - Backward control is active
    if (packet.js1X < FWD_TREHOLD || packet.js1X > BWD_TREHOLD)
    {
      // Forward
      if (packet.js1X < FWD_TREHOLD)
      {
        // Left is also active
        if (packet.js2Y > L_TREHOLD)
        {
          // Rotate the front servos
        }
        // Right is also active
        else if (packet.js2Y < R_TREHOLD)
        {
          // Rotate the front servos
        }
        // Only forward is active
        else
        {
          currSpeed = map(packet.js1X, 0, FWD_TREHOLD - 1, SC_MAX_SPEED, SC_MIN_SPEED);
          setAllMotorsDir(MOTOR_DIR_FWD);
          setAllMotorsSpeed(currSpeed);
          Serial.println("Forward");
        }
      }
      // Backward
      else if (packet.js1X > BWD_TREHOLD)
      {
        // Left is also active
        if (packet.js2Y > L_TREHOLD)
        {
          // Rotate all servos
        }
        // Right is also active
        else if (packet.js2Y < R_TREHOLD)
        {
          // Rotate all servos
        }
        // Only backward is active
        else
        {
          currSpeed = -1 * map(packet.js1X, BWD_TREHOLD + 1, 1023, SC_MIN_SPEED, SC_MAX_SPEED);
          setAllMotorsDir(MOTOR_DIR_BWD);
          setAllMotorsSpeed(currSpeed);
          Serial.println("Backward");
        }
      }      
    }


    // Forward - Backward control is inactive
    if (packet.js1X >= FWD_TREHOLD && packet.js1X <= BWD_TREHOLD)
    {
      // If only left is active
      if (packet.js2Y > L_TREHOLD)
      {

      }
      // If only right is active
      else if (packet.js2Y < R_TREHOLD)
      {

      }
      // No joystick is active
      else
      {
        setAllMotorsSpeed(0);
        Serial.println("Stop");
      }
    }
  }
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