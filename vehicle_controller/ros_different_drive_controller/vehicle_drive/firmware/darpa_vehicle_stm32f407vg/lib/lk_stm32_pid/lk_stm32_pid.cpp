#include "lk_stm32_pid.h"

/**
 * This class allows PID speed (degrees/second) control of motors.
 * 
 * Each instance requires one Motor and one Encoder objects (one for each motor) to work. This system reads speed from 
 * the Encoder object and calculates the error between the set point (desired speed, set using setSpeed()) and process 
 * variable (current measured speed).
 * 
 * This error is then fed into the PID equation and it outputs the PWM adjustment value. This PWM value is then used 
 * to set the motor's PWM using Motor object.
 * 
 * By calling correctPwm() every deltaTime (in Encoder class), a new process variable is fed back into this system 
 * where a new PWM adjustment value is calculated and used to set motor PWM. This loop process continues until the set 
 * point is equal to process variable (desire speed is met).
 * 
 */

/**
 * @brief Construct a new Speed Control:: Speed Control object.
 * 
 * @param motor Motor object used.
 * @param encoder Encoder object used.
 */
PIDControl::PIDControl(Motor *motor, Encoder *encoder) : _motor(motor), _encoder(encoder)
{
  pwm = 0;
  setPoint = 0;
  prevSpeed = 0;
  minSpeed = DEF_MIN_SPEED;
  pGain = DEF_GAIN;
  iGain = DEF_GAIN;
  dGain = DEF_GAIN;
  iTerm = 0;
}

/**
 * @brief Corrects the PWM of the motor to maintain the speed of set point.
 * @details This function must be called every deltaTime (in Encoder class).
 * 
 */
int PIDControl::correctPwm(void)
{
  // Get the scalar speed.
  long speed = _encoder->getSpeed();
  if (speed < 0)
  {
    speed *= -1;
  }

  // Find the error.
  long error = setPoint - speed;

  // Calculate P, I, D values.
  double pTerm = pGain * (double)error;
  iTerm += iGain * (double)error;
  double dTerm = dGain * (speed - prevSpeed); // Uses derivative on measurement. See: http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/

  // Calculate the pwm value to set.
  int correction = pTerm + iTerm - dTerm;
  pwm += correction;
  if (pwm > 100)
  {
    pwm = 100;
  }
  if (pwm < 0)
  {
    pwm = 0;
  }
   
  // Set pwm.
  _motor->setPwm(pwm);
  prevSpeed = speed;

  return speed;
}

/**
 * @brief Sets the P, I, D gains of motors.
 * 
 * @param pGain Proportional gain of motor.
 * @param iGain Integral gain of motor.
 * @param dGain Derivative gain of motor.
 */
void PIDControl::setGains(
      double pGain, 
      double iGain, 
      double dGain
      )
{
  this->pGain = pGain;
  this->iGain = iGain;
  this->dGain = dGain;
}

/**
 * @brief Sets the set point speed (in degrees/second) of motor.
 * 
 * @param speed Value to set the set point with.
 */
void PIDControl::setSpeed(long speed)
{
  if (speed < 0)
  {
    _motor->setDir(LK_MOTOR_BWD);
    speed *= -1;
  }
  else if (speed > 0)
  {
    _motor->setDir(LK_MOTOR_FWD);
  }
  else
  {
    _motor->stop();
    _motor->setPwm(0);
  }

  if (speed > 0 && speed < minSpeed){
    speed = minSpeed;
  }

  setPoint = speed;
}

/**
 * @brief Sets the minimum speed to run.
 * 
 * @param speed Value to set the minimum speed with.
 */
void PIDControl::setMinSpeed(long speed)
{
  if (speed < 0)
  {
    speed = 0;
  }
  if (speed > 255)
  {
    speed = 255;
  }

  minSpeed = speed;
}