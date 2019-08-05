#include <Motor.h>

/**
 * @brief Construct a new Motor:: Motor object.
 * 
 * @param pinDir Pin number for direction pin.
 * @param pinPwm Pin number for PWM pin.
 */
Motor::Motor(uint8_t pinDir, uint8_t pinPwm) : _pinDir(pinDir), _pinPwm(pinPwm)
{
  pinMode(pinDir, OUTPUT);
  pinMode(pinPwm, OUTPUT);
  
}

/**
 * @brief Sets the direction of motor.
 * 
 * @param dir The direction can be MOTOR_DIR_FWD (forward) or MOTOR_DIR_BWD (backward).
 * @return MOTOR_RESULT_t MOTOR_SUCCESS if set successful or MOTOR_FAILURE if set failure due to invalid direction.
 */
MOTOR_RESULT_t Motor::setDir(MOTOR_DIR_t dir)
{
  if (dir != MOTOR_DIR_FWD && dir != MOTOR_DIR_BWD)
  {
    return MOTOR_FAILURE;
  }

  if (_isInverted)
  {
    dir = (dir == MOTOR_DIR_FWD) ? MOTOR_DIR_BWD : MOTOR_DIR_FWD;
  }

  _dir = dir;

  if (dir == MOTOR_DIR_FWD)
  {
    digitalWrite(_pinDir, LOW);
  }
  else
  {
    digitalWrite(_pinDir, HIGH);
  }

  return MOTOR_SUCCESS;
}

/**
 * @brief Get the direction of motor.
 * 
 * @return MOTOR_DIR_t Returned direction can be MOTOR_DIR_FWD (forward), MOTOR_DIR_BWD (backward).
 */
MOTOR_DIR_t Motor::getDir(void)
{
  return _dir;
}

/**
 * @brief Sets the PWM value of motor. Does not start the motor if it is already stopped.
 * 
 * @param pwm The PWM value to be set. Range is from 0 - 255.
 * @return MOTOR_RESULT_t MOTOR_SUCCESS if set successful or MOTOR_FAILURE if set failure due to PWM out of range.
 */
MOTOR_RESULT_t Motor::setPwm(uint8_t pwm)
{
  if (pwm > 255)
  {
    return MOTOR_FAILURE;
  }

  _pwm = pwm;

  if (!_isStopped)
  {
    analogWrite(_pinPwm, pwm);
  }

  return MOTOR_SUCCESS;
}

/**
 * @brief Get the PWM value of motor.
 * 
 * @return uint8_t Returned PWM value ranges from 0 - 255.
 */
uint8_t Motor::getPwm(void)
{
  return _pwm;
}

/**
 * @brief Starts rotation of motor.
 * 
 * @return MOTOR_RESULT_t MOTOR_SUCCESS if start successful or MOTOR_FAILURE if start failure due to already running motor.
 */
MOTOR_RESULT_t Motor::start(void)
{
  if (!_isStopped)
  {
    return MOTOR_FAILURE;
  }

  _isStopped = false;
  analogWrite(_pinPwm, _pwm);

  return MOTOR_SUCCESS;
}

/**
 * @brief Stops rotation of motor.
 * 
 * @return MOTOR_RESULT_t MOTOR_SUCCESS if stop successful or MOTOR_FAILURE if stop failure due to already stopped motor.
 */
MOTOR_RESULT_t Motor::stop(void)
{
  if (_isStopped)
  {
    return  MOTOR_FAILURE;
  }

  _isStopped = true;
  analogWrite(_pinPwm, 0);
  
  return MOTOR_SUCCESS;
}

/**
 * @brief Inverts the direction of the motor. Useful when mounting motors in different orientation.
 * 
 * @param inv Boolean value to indicate if direction should invert.
 */
void Motor::invertDir(bool inv)
{
  _isInverted = inv;
}