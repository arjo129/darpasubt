/*
 * lk_pwm.c
 *
 *  Created on: Jul 25, 2019
 *      Author: kokteng
 */

#include "lk_pwm.h"

void LK_PWM_Init(
    TIM_HandleTypeDef* timHandle,
    TIM_TypeDef* TIMx,
    uint32_t channel,
    uint32_t prescaler,
    uint32_t counterMode,
    uint32_t period,
    uint32_t clockDivision,
    uint32_t autoReloadPreload,
    uint32_t ocMode,
    uint32_t ocPolarity,
    uint32_t ocFastMode
    )
{
  // Declare config variables.
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  // Initialise
  timHandle->Instance = TIMx;
  timHandle->Init.Prescaler = prescaler;
  timHandle->Init.CounterMode = counterMode;
  timHandle->Init.Period = period;
  timHandle->Init.ClockDivision = clockDivision;
  timHandle->Init.AutoReloadPreload = autoReloadPreload;
  HAL_TIM_PWM_Init(timHandle);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(timHandle, &sMasterConfig);

  sConfigOC.OCMode = ocMode;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = ocPolarity;
  sConfigOC.OCFastMode = ocFastMode;
  HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, channel);
}

void LK_PWM_SetPWM(
    TIM_HandleTypeDef* timHandle,
    uint32_t channel,
    uint32_t ocMode,
    uint32_t ocPolarity,
    uint32_t ocFastMode,
    uint16_t value
    )
{

  TIM_OC_InitTypeDef sConfigOC;

  sConfigOC.OCMode = ocMode;
  sConfigOC.Pulse = value;
  sConfigOC.OCPolarity = ocPolarity;
  sConfigOC.OCFastMode = ocFastMode;
  HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, channel);

  if (value > 0)
  {
    HAL_TIM_PWM_Start(timHandle, channel);
  }
  else
  {
    HAL_TIM_PWM_Stop(timHandle, channel);
  }
}
