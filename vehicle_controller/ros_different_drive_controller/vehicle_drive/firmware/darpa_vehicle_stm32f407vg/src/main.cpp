/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

// Macros Definitions
#define K_P 0.001667
#define K_I 0
#define K_D 0

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;
LK_PWM_DATA_t pwmHandle;
// Motor A PID control handles.
// Motor motorA(GPIO_PIN_14, GPIO_PIN_6, GPIOD, GPIOC);
// Encoder encoderA(GPIO_PIN_8, GPIO_PIN_10, GPIOD, GPIOD, 125000);
// PIDControl pidA(&motorA, &encoderA);
// Motor B PID control handles.
// Motor motorA(GPIO_PIN_14, GPIO_PIN_12, GPIOD, GPIOD);
// Encoder encoderA(GPIO_PIN_8, GPIO_PIN_10, GPIOD, GPIOD, 125000);
// PIDControl pidA(&motorA, &encoderA);
double p;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void TM_RTC_WakeupHandler(void);
void TM_EXTI_Handler(uint16_t GPIO_Pin);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Init system clock for maximum system speed */
  // TM_RCC_InitSystem();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD13 PD14 PD15 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim4.Instance = TIM3;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 8399;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 4199;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim4);
  
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);


  // Initialise Orange indicator LED.
  // TM_GPIO_Init(GPIOD, GPIO_PIN_13, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
  // TM_GPIO_SetPinLow(GPIOD, GPIO_PIN_13);

  // Initialise PWM speed indicator LED (Blue).
  // TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low, GPIO_AF2_TIM4);
  // TM_GPIO_SetPinAsAlternate(GPIOD, GPIO_PIN_15);

  // Initialise the PWM signal generator.
  // LK_PWM_Init(
  //     &pwmHandle,
  //     &htim4,
  //     TIM4,
  //     TIM_CHANNEL_4,
  //     0,
  //     TIM_COUNTERMODE_UP,
  //     8399,
  //     TIM_CLOCKDIVISION_DIV1,
  //     TIM_AUTORELOAD_PRELOAD_DISABLE,
  //     TIM_OCMODE_PWM2,
  //     TIM_OCPOLARITY_LOW,
  //     TIM_OCFAST_ENABLE
  //     );
  // LK_PWM_SetPwmPercent(&pwmHandle, 0);

  // ISSUE: TM's Delay library seems to clash with RCC/RTC library, timing becomes off. Investigate/debug further.
  // TM_DELAY_Init();

  /* Init RTC */
  // if (TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
  //   /* RTC was already initialized and time is running */
  //   TM_GPIO_SetPinHigh(GPIOD, GPIO_PIN_13);
  //   TM_RTC_Interrupts(TM_RTC_Int_125ms);
  // } else {
  //   /* RTC was now initialized */
  // }

  // Set motor and PID parameters.  
  // motorA.setDir(LK_MOTOR_BWD);
  // motorA.setPwm(40);
  // motorA.start();
  // pidA.setGains(K_P, K_I, K_D);
  // pidA.setMinSpeed(0);
  // pidA.setSpeed(20000); // Speed is in deg/sec. Max for this motor is 60000.

  while (1)
  {
  //   Delay(50000);
  //   TM_GPIO_TogglePinValue(GPIOD, GPIO_PIN_13);

  //   // int speed = pidA.correctPwm();
  //   int speed = encoderA.getSpeed();
  //   encoderA.getDistance();
  //   p = ((double)speed / 60000.0) * 100.0;
  //   if (speed >= 0)
  //   {
  //     LK_PWM_SetPwmPercent(&pwmHandle, p);
  //   }
  //   else
  //   {
  //     LK_PWM_SetPwmPercent(&pwmHandle, p * -1.0);
  //   }
  MX_USB_HOST_Process();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

/* Called when wakeup interrupt happens */
void TM_RTC_WakeupHandler(void) {
  /* Toggle leds */
  // TM_GPIO_TogglePinValue(GPIOD, GPIO_PIN_13);

  // int speed = pidA.correctPwm();
  // int speed = encoderA.getSpeed();
  // // encoderA.getDistance();
  // p = ((double)speed / 60000.0) * 100.0;
  // if (speed >= 0)
  // {
  //   LK_PWM_SetPwmPercent(&pwmHandle, p);
  // }
  // else
  // {
  //   LK_PWM_SetPwmPercent(&pwmHandle, p * -1.0);
  // }
}

/* Handle all EXTI lines */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
  /* Check proper line */
  // if (GPIO_Pin == GPIO_PIN_8) {
  //   encoderA.updateCount();
  // }
}