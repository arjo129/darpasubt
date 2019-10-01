#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include <Motor.h>
#include <Encoder.h>

#define DIR_L 7
#define PWM_L 6
#define DIR_R 8
#define PWM_R 9
#define ENC_L_A 2
#define ENC_L_B 4
#define ENC_R_A 3
#define ENC_R_B 5

// Function prototypes
void setLeftMotor(const std_msgs::Float32 &msg);
void setRightMotor(const std_msgs::Float32 &msg);
void updateLeftEnc(void);
void updateRightEnc(void);

// Motors/Encoders related
Motor motor(DIR_L, PWM_L, DIR_R, PWM_R);
Encoder encoderL(ENC_L_A, ENC_L_B);
Encoder encoderR(ENC_R_A, ENC_R_B);

// ROS related
std_msgs::Int16 leftEncMsg;
std_msgs::Int16 rightEncMsg;
ros::NodeHandle nh;
ros::Publisher pubEncLeft("lwheel", &leftEncMsg);
ros::Publisher pubEncRight("rwheel", &rightEncMsg);
ros::Subscriber<std_msgs::Float32> subMotorLeft("lmotor_cmd", &setLeftMotor);
ros::Subscriber<std_msgs::Float32> subMotorRight("rmotor_cmd", &setRightMotor);

void setup() {
  // Motors/Encoders
  pinMode(DIR_L, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(ENC_L_A, INPUT);
  pinMode(ENC_R_A, INPUT);
  pinMode(ENC_L_B, INPUT);
  pinMode(ENC_R_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_L_A), updateLeftEnc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_L_B), updateRightEnc, CHANGE);
  motor.setM1Dir(DIR_FWD);
  motor.setM2Dir(DIR_FWD);
  motor.setM1Pwm(0);
  motor.setM2Pwm(0);
  motor.startM1();
  motor.startM2();

  // ROS
  nh.initNode();
  nh.advertise(pubEncLeft);
  nh.advertise(pubEncRight);
  nh.subscribe(subMotorLeft);
  nh.subscribe(subMotorRight);
}

void loop() {
  leftEncMsg.data = encoderL.getCount();
  rightEncMsg.data = encoderR.getCount();
  pubEncLeft.publish(&leftEncMsg);
  pubEncRight.publish(&rightEncMsg);

  nh.spinOnce();
}

/**
 * @brief Set the Left Motor object PWM value.
 * 
 * @param msg message containing PWM value.
 */
void setLeftMotor(const std_msgs::Float32 &msg)
{
  if (msg.data > 0)
  {
    motor.setM1Dir(DIR_FWD);
  }
  else if (msg.data < 0)
  {
    motor.setM1Dir(DIR_BWD);
  }
  else
  {
    // Do nothing.
  }
  
  motor.setM1Pwm(msg.data);
}

/**
 * @brief Set the Right Motor object PWM value.
 * 
 * @param msg message containing PWM value.
 */
void setRightMotor(const std_msgs::Float32 &msg)
{
  if (msg.data > 0)
  {
    motor.setM2Dir(DIR_FWD);
  }
  else if (msg.data < 0)
  {
    motor.setM2Dir(DIR_BWD);
  }
  else
  {
    // Do nothing.
  }
  
  motor.setM2Pwm(msg.data);
}

/**
 * @brief Updates the left encoder tick count.
 * 
 */
void updateLeftEnc(void)
{
  encoderL.updateCount();
}

/**
 * @brief Updates the right encoder tick count.
 * 
 */
void updateRightEnc(void)
{
  encoderR.updateCount();
}