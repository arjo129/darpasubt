/*
 * lk_pwm.c
 *
 *  Created on: Jul 25, 2019
 *      Author: kokteng
 */

#include "lk_pwm.h"

/**
 * @brief Initialises PWM generator with the given parameters.
 * @param pwmData pointer to LK_PWM_DATA_t struct containing PWM information data.
 * @param timHandle pointer to IM_HandleTypeDef struct containing TIM handle information.
 * @param TIMx pointer to TIM_TypeDef defining the TIM to use.
 * @param channel specifies channel of TIM to use.
 * @param prescaler specifies timer prescaler to use.
 * @param counterMode specifies timer counter mode to use.
 * @param period specifies timer period to use.
 * @param clockDivision specifies timer clock division to use.
 * @param autoReloadPreload enable or disable timer auto-reload preload..
 * @param ocMode specifies TIM Output Compare and PWM modes to use.
 * @param ocPolarity specifies TIM_OCPOLARITY_LOW or TIM_OCPOLARITY_HIGH for low or high polarity.
 * @param ocFastMode specifies TIM_OCFAST_ENABLE or TIM_OCFAST_DISABLE to set FastMode.
 */
void LK_PWM_Init(
    LK_PWM_DATA_t* pwmData,
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

  // Initialise TIM PWM.
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
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, channel);

  // Save all parameters as a struct.
  pwmData->timHandle = timHandle;
  pwmData->channel = channel;
  pwmData->period = period;
  pwmData->prescaler = prescaler;
  pwmData->counterMode = counterMode;
  pwmData->clockDivision = clockDivision;
  pwmData->autoReloadPreload = autoReloadPreload;
  pwmData->ocMode = ocMode;
  pwmData->ocPolarity = ocPolarity;
  pwmData->ocFastMode = ocFastMode;
}

/**
 * @brief Set the PWM signal duty cycle absolute value.
 * @param pwmData pointer to LK_PWM_DATA_t struct containing PWM information data.
 * @param value Absolute PWM value to set.
 */
void LK_PWM_SetPwm(LK_PWM_DATA_t* pwmData, uint32_t value)
{
  TIM_OC_InitTypeDef sConfigOC;

  sConfigOC.OCMode = pwmData->ocMode;
  sConfigOC.Pulse = value;
  sConfigOC.OCPolarity = pwmData->ocPolarity;
  sConfigOC.OCFastMode = pwmData->ocFastMode;
  HAL_TIM_PWM_ConfigChannel(pwmData->timHandle, &sConfigOC, pwmData->channel);

  if (value > 0)
  {
    HAL_TIM_PWM_Start(pwmData->timHandle, pwmData->channel);
  }
  else
  {
    HAL_TIM_PWM_Stop(pwmData->timHandle, pwmData->channel);
  }
}

/**
 * @brief Set the PWM signal duty cycle percentage.
 * @param pwmData pointer to LK_PWM_DATA_t struct containing PWM information data.
 * @param percent Percentage PWM value to set.
 */
void LK_PWM_SetPwmPercent(LK_PWM_DATA_t* pwmData, double percent)
{
  if (percent > 100)
  {
    LK_PWM_SetPwm(pwmData, pwmData->period);
  }
  else if (percent <= 0)
  {
    LK_PWM_SetPwm(pwmData, 0);
  }

  LK_PWM_SetPwm(pwmData, (uint32_t)((percent / 100.0) * (double)(pwmData->period)));
}
