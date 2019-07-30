#include "lk_stm32_motor.h"

/**
 * This class allows creation of Motor objects that controls the direction and rotation speed of a DC brushed motor.
 * 
 * Each instantiation controls single DC motor.
 * 
 * To begin rotating the motor:
 *  1. Initialise the pins required to drive direction and PWM of the motor by instantiating the class.
 *  2. Set the direction with setDir().
 *  3. Set the PWM value with setPwm().
 *  4. Begint rotation with start(). The motor will not rotate until this function is called for the first time.
 */

/**
 * @brief Construct a new Motor:: Motor object.
 * 
 * @param pinDir Pin number for motor direction pin.
 * @param pinPwm Pin number for motor PWM pin.
 * @param pinDirPort Pointer to GPIO Port type definition that corresponds to pinDir.
 * @param pinPwmPort Pointer to GPIO Port type definition that corresponds to pinPwm.
 */
Motor::Motor(
  uint16_t pinDir, 
  uint16_t pinPwm, 
  GPIO_TypeDef* pinDirPort, 
  GPIO_TypeDef* pinPwmPort
  ) : _pinDir(pinDir), _pinPwm(pinPwm), _pinDirPort(pinDirPort), _pinPwmPort(pinPwmPort)
{
  // Initialises the PWM to be used for motor PWM speed control.
  LK_PWM_Init(
      &_pwmHandle,
      &_timHandle,
      TIM3,
      TIM_CHANNEL_2,
      0,
      TIM_COUNTERMODE_UP,
      8399,
      TIM_CLOCKDIVISION_DIV1,
      TIM_AUTORELOAD_PRELOAD_DISABLE,
      TIM_OCMODE_PWM1,
      TIM_OCPOLARITY_LOW,
      TIM_OCFAST_ENABLE
      );

  // Initialises the GPIO to be used for directional and PWM control.
  TM_GPIO_Init(_pinDirPort, _pinDir, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
  TM_GPIO_SetPinLow(_pinDirPort, _pinDir);
  TM_GPIO_InitAlternate(_pinPwmPort,  _pinPwm, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low, GPIO_AF2_TIM3);
  TM_GPIO_SetPinAsAlternate(_pinPwmPort, _pinPwm);

  LK_PWM_SetPwmPercent(&_pwmHandle, 0);
}

/**
 * @brief Sets the direction of motor channel.
 * 
 * @param dir The direction can be LK_MOTOR_FWD (forward) or LK_MOTOR_BWD (backward).
 * @return LK_MOTOR_RESULT_t LK_MOTOR_SUCCESS if set successful or LK_MOTOR_FAIL if set failure due to invalid direction.
 */
LK_MOTOR_RESULT_t Motor::setDir(LK_MOTOR_DIR_t dir)
{
  if (dir != LK_MOTOR_FWD && dir != LK_MOTOR_BWD)
  {
    return LK_MOTOR_FAIL;
  }

  _dir = dir;

  if (dir == LK_MOTOR_FWD)
  {
    TM_GPIO_SetPinLow(_pinDirPort, _pinDir);
  }
  else
  {
    TM_GPIO_SetPinHigh(_pinDirPort, _pinDir);
  }

  return LK_MOTOR_SUCCESS;
}

/**
 * @brief Get the direction of motor channel.
 * 
 * @return int Returned direction can be LK_MOTOR_FWD (forward), LK_MOTOR_BWD (backward).
 */
LK_MOTOR_DIR_t Motor::getDir(void)
{
  return _dir;
}


/**
 * @brief Sets the PWM value of motor channel. Does not start the motor if it is already stopped.
 * 
 * @param pwm The PWM value to be set. Range is from 0 - 100 percent.
 * @return LK_MOTOR_RESULT_t LK_MOTOR_SUCCESS if set successful or LK_MOTOR_FAIL if set failure due to PWM out of range.
 */
LK_MOTOR_RESULT_t Motor::setPwm(uint16_t pwm)
{
  if (pwm < 0 || pwm > 100)
  {
    return LK_MOTOR_FAIL;
  }

  _pwm = pwm;

  if (!_isStopped)
  {
    LK_PWM_SetPwmPercent(&_pwmHandle, pwm);
  }

  return LK_MOTOR_SUCCESS;
}

/**
 * @brief Get the PWM value of motor channel.
 * 
 * @return uint8_t Returned PWM value ranges from 0 - 100 percent.
 */
uint8_t Motor::getPwm(void)
{
  return _pwm;
}

/**
 * @brief Starts rotation of motor channel.
 * 
 * @return LK_MOTOR_RESULT_t LK_MOTOR_SUCCESS if start successful or LK_MOTOR_FAIL if start failure due to already running motor.
 */
LK_MOTOR_RESULT_t Motor::start(void)
{
  if (!_isStopped)
  {
    return LK_MOTOR_FAIL;
  }

  _isStopped = false;
  LK_PWM_SetPwmPercent(&_pwmHandle, _pwm);
  
  return LK_MOTOR_SUCCESS;
}

/**
 * @brief Stops rotation of motor channel.
 * 
 * @return LK_MOTOR_RESULT_t LK_MOTOR_SUCCESS if stop successful or LK_MOTOR_FAIL if stop failure due to already stopped motor.
 */
LK_MOTOR_RESULT_t Motor::stop(void)
{
  if (_isStopped)
  {
    return  LK_MOTOR_FAIL;
  }

  _isStopped = true;
  LK_PWM_SetPwmPercent(&_pwmHandle, _pwm);
  
  return LK_MOTOR_SUCCESS;
}

