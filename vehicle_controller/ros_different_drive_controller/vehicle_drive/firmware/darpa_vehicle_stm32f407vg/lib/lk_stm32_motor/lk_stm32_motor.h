#ifndef __LK_MOTOR_H
#define __LK_MOTOR_H

#include "lk_pwm.h"
#include "tm_stm32_gpio.h"

// Default pin values
#define DEF_DIR_PIN GPIO_PIN_14
#define DEF_DIR_PORT GPIOD
#define DEF_PWM_PIN GPIO_PIN_12
#define DEF_PWM_PORT GPIOD

#ifdef __cplusplus

/**
 * @brief Motor direction mode, forward (FWD) or backward (BWD).
 * 
 */
typedef enum {
  LK_MOTOR_FWD = 0x01,
  LK_MOTOR_BWD = 0x02
} LK_MOTOR_DIR_t;

/**
 * @brief Motor operations results.
 * 
 */
typedef enum {
  LK_MOTOR_SUCCESS = 0,
  LK_MOTOR_FAIL = 0
} LK_MOTOR_RESULT_t;

class Motor
{
  public:
    Motor(
      uint16_t pinDir = DEF_DIR_PIN,
      uint16_t pinPwm = DEF_PWM_PIN,
      GPIO_TypeDef* pinDirPort = DEF_DIR_PORT, 
      GPIO_TypeDef* pinPwmPort = DEF_PWM_PORT
      );
    LK_MOTOR_RESULT_t setDir(LK_MOTOR_DIR_t dir);
    LK_MOTOR_DIR_t getDir(void);
    LK_MOTOR_RESULT_t setPwm(uint16_t pwm);
    uint8_t getPwm(void);
    LK_MOTOR_RESULT_t start(void);
    LK_MOTOR_RESULT_t stop(void);


  private:
    uint16_t _pinDir; // Direction pin.
    uint16_t _pinPwm; // PWM pin.
    GPIO_TypeDef* _pinDirPort; // Direction pin PORT.
    GPIO_TypeDef* _pinPwmPort; // PWM pin PORT.
    
    LK_MOTOR_DIR_t _dir; // Direction.
    int _pwm; // PWM value.

    bool _isStopped = true; // Flag to check if motor is stopped.

    TIM_HandleTypeDef _timHandle;
    LK_PWM_DATA_t _pwmHandle;
};

#endif

#endif /* __LK_MOTOR_H */