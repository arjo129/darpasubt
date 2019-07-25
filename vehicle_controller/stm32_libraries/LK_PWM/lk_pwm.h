/*
 * lk_pwm.h
 *
 *  Created on: Jul 25, 2019
 *      Author: kokteng
 */

#ifndef LK_PWM_LK_PWM_H_
#define LK_PWM_LK_PWM_H_

#include "stm32f4xx_hal.h"

typedef struct {
  TIM_TypeDef* TIM;
  uint32_t channel;
  uint32_t period;
  uint32_t prescaler;
  uint32_t counterMode;
  uint32_t period;
  uint32_t clockDivision;
  uint32_t autoReloadPreload;
  uint32_t ocMode;
  uint32_t ocPolarity;
  uint32_t ocFastMode;
} LK_PWM_DATA_t;

void LK_PWM_Init(
    LK_PWM_DATA_t* pwmData
    );
void LK_PWM_SetPWM(
    LK_PWM_DATA_t* pwmData,
    uint16_t value
    );

#endif /* LK_PWM_LK_PWM_H_ */
