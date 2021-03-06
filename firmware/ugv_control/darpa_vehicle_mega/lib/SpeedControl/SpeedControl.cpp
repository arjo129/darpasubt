#include <SpeedControl.h>

/**
 * This class allows PID speed (degrees/second) control of motors.
 * 
 * Each instance requires a Motor and an Encoder object to work. This system reads speed from the Encoder object and 
 * calculates the error between the set point (desired speed, set using setSpeed()) and process variable (current 
 * measured speed).
 * 
 * This error is then fed into the PID equation and it outputs the PWM adjustment value. This PWM value is then used 
 * to set the motor's PWM using Motor object.
 * 
 * By calling correctPwm() every deltaTime (in Encoder class), a new process variable is fed back into this system 
 * where a new PWM adjustment value is calculated and used to set motor PWM. This loop process continues until the set 
 * point is equal to process variable (desire speed is met).
 * 
 * NOTE: Depending on the Arduino you are using (Uno, Due, Zero, etc.), int can be stored as 2-bytes or 4-bytes values.
 * In certain cases, you may wish to replace long with int to save memory. Correspondingly, be wary with the fact that
 * int are stored as 2-bytes on some boards (eg. Uno). You may then want to consider overflow cases. 
 */

/**
 * @brief Construct a new Speed Control object.
 * 
 * @param motor Motor object used.
 * @param encoder Encoder object used.
 */
SpeedControl::SpeedControl(Motor *motor, Encoder *encoder) : motor(motor), encoder(encoder) {
  pwm = 0;
  setPoint = 0;
  prevSpeed = 0;
  minSpeed = DEF_MIN_SPEED;
  pGain = DEF_GAIN;
  iGain = DEF_GAIN;
  dGain = DEF_GAIN;
  iTerm = 0;
};

/**
 * @brief Corrects the PWM of the motor to maintain the speed of set point.
 * @details This function must be called every deltaTime (in Encoder class).
 * 
 */
long SpeedControl::correctPwm(void)
{
  // Get the scalar speed.
  long speed = encoder->getSpeed();
  long retSpeed = speed;
  if (speed < 0)
  {
    speed *= -1;
  }

  // Find the error.
  long error = setPoint - speed;

  // Calculate P, I, D values.
  // TODO: Explore better Integral term computation (See: sliding window values)
  double pTerm = pGain * (double)error;
  setPoint == 0 ? iTerm = 0 : iTerm += iGain * (double)error;
  double dTerm = dGain * (speed - prevSpeed); // Uses derivative on measurement. See: http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/

  // Calculate the pwm value to set.
  long correction = pTerm + iTerm - dTerm;
  pwm += correction;
  if (pwm > 255)
  {
    pwm = 250;
  }
  if (pwm < 0)
  {
    pwm = 0;
  }
  if (setPoint == 0 && speed == 0) pwm = 0;
   
  // Set pwm.
  motor->setPwm(pwm);
  prevSpeed = speed;

  return retSpeed;
}

/**
 * @brief Sets the P, I, D gains.
 * 
 * @param pGain Proportional gain.
 * @param iGain Integral gain.
 * @param dGain Derivative gain.
 */
void SpeedControl::setGains(double pGain, double iGain, double dGain)
{
  this->pGain = pGain;
  this->iGain = iGain;
  this->dGain = dGain;
}

/**
 * @brief Sets the set point speed (in degrees/second).
 * 
 * @param speed Value to set the set point with.
 */
void SpeedControl::setSpeed(long speed)
{
  if (speed < 0)
  {
    motor->setDir(MOTOR_DIR_BWD);
    motor->start();
  }
  else if (speed > 0)
  {
    motor->setDir(MOTOR_DIR_FWD);
    motor->start();
  }
  else
  {
    motor->stop();
    motor->setPwm(0);
  }
  if ((speed > 0 && speed < minSpeed) || (speed < 0 && speed > -minSpeed)) {
    setPoint = minSpeed;
  }
  else if (speed < 0)
  {
    setPoint = -speed;
  }
  else
  {
    setPoint = speed;
  }
}

/**
 * @brief Sets the minimum speed to run.
 * 
 * @param speed Value to set the minimum speed with.
 */
void SpeedControl::setMinSpeed(long speed)
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
