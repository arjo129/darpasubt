#include <Motor.h>

/**
 * @brief Construct a new Motor:: Motor object.
 * 
 * @param pinDir1 Pin number for first motor direction pin.
 * @param pinPwm1 Pin number for first motor PWM pin.
 * @param pinDir2 Pin number for second motor direction pin.
 * @param pinPwm2 Pin number for second motor PWM pin.
 */
Motor::Motor(int pinDir1, int pinPwm1, int pinDir2, int pinPwm2) : pinDir1(pinDir1), pinPwm1(pinPwm1), pinDir2(pinDir2), pinPwm2(pinPwm2) {}

/**
 * @brief Sets the direction of first motor channel.
 * 
 * @param dir The direction can be DIR_FWD (forward) or DIR_BWD (backward).
 * @return int SUCCESS if set successful or FAIL if set failure due to invalid direction.
 */
int Motor::setM1Dir(int dir)
{
  if (dir != DIR_FWD && dir != DIR_BWD)
  {
    return FAIL;
  }

  dir1 = dir;

  if (dir == DIR_FWD)
  {
    digitalWrite(pinDir1, LOW);
  }
  else
  {
    digitalWrite(pinDir1, HIGH);
  }

  return SUCCESS;
}

/**
 * @brief Sets the direction of second motor channel.
 * 
 * @param dir The direction can be DIR_FWD (forward) or DIR_BWD (backward).
 * @return int SUCCESS if set successful or FAIL if set failure due to invalid direction.
 */
int Motor::setM2Dir(int dir)
{
  if (dir != DIR_FWD && dir != DIR_BWD)
  {
    return FAIL;
  }

  dir2 = dir;

  if (dir == DIR_FWD)
  {
    digitalWrite(pinDir2, LOW);
  }
  else
  {
    digitalWrite(pinDir2, HIGH);
  }

  return SUCCESS;
}

/**
 * @brief Get the direction of first motor channel.
 * 
 * @return int Returned direction can be DIR_FWD (forward), DIR_BWD (backward).
 */
int Motor::getM1Dir(void)
{
  return dir1;
}

/**
 * @brief Get the direction of second motor channel.
 * 
 * @return int Returned direction can be DIR_FWD (forward), DIR_BWD (backward).
 */
int Motor::getM2Dir(void)
{
  return dir2;
}

/**
 * @brief Sets the PWM value of first motor channel. Does not start the motor if it is already stopped.
 * 
 * @param pwm The PWM value to be set. Range is from 0 - 255.
 * @return int SUCCESS if set successful or FAIL if set failure due to PWM out of range.
 */
int Motor::setM1Pwm(int pwm)
{
  if (pwm < 0 || pwm > 255)
  {
    return FAIL;
  }

  pwm1 = pwm;

  if (!isStopped1)
  {
    analogWrite(pinPwm1, pwm);
  }

  return SUCCESS;
}

/**
 * @brief Sets the PWM value of second motor channel. Does not start the motor if it is already stopped.
 * 
 * @param pwm The PWM value to be set. Range is from 0 - 255.
 * @return int SUCCESS if set successful or FAIL if set failure due to PWM out of range.
 */
int Motor::setM2Pwm(int pwm)
{
  if (pwm < 0 || pwm > 255)
  {
    return FAIL;
  }

  pwm2 = pwm;

  if (!isStopped2)
  {
    analogWrite(pinPwm2, pwm);
  }

  return SUCCESS;
}

/**
 * @brief Get the PWM value of first motor channel.
 * 
 * @return int Returned PWM value ranges from 0 - 255.
 */
int Motor::getM1Pwm(void)
{
  return pwm1;
}

/**
 * @brief Get the PWM value of second motor channel.
 * 
 * @return int Returned PWM value ranges from 0 - 255.
 */
int Motor::getM2Pwm(void)
{
  return pwm2;
}

/**
 * @brief Starts rotation of first motor channel.
 * 
 * @return int SUCCESS if start successful or FAIL if start failure due to already running motor.
 */
int Motor::startM1(void)
{
  if (!isStopped1)
  {
    return FAIL;
  }

  isStopped1 = false;
  analogWrite(pinPwm1, pwm1);

  return SUCCESS;
}

/**
 * @brief Starts rotation of second motor channel.
 * 
 * @return int SUCCESS if start successful or FAIL if start failure due to already running motor.
 */
int Motor::startM2(void)
{
  if (!isStopped2)
  {
    return FAIL;
  }

  isStopped2 = false;
  analogWrite(pinPwm2, pwm2);

  return SUCCESS;
}

/**
 * @brief Stops rotation of first motor channel.
 * 
 * @return int SUCCESS if stop successful or FAIL if stop failure due to already stopped motor.
 */
int Motor::stopM1(void)
{
  if (isStopped1)
  {
    return  FAIL;
  }

  isStopped1 = true;
  analogWrite(pinPwm1, 0);
  
  return SUCCESS;
}

/**
 * @brief Stops rotation of second motor channel.
 * 
 * @return int SUCCESS if stop successful or FAIL if stop failure due to already stopped motor.
 */
int Motor::stopM2(void)
{
  if (isStopped2)
  {
    return  FAIL;
  }

  isStopped2 = true;
  analogWrite(pinPwm2, 0);
  
  return SUCCESS;
}
