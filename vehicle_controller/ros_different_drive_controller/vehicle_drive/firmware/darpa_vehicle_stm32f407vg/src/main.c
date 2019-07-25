/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;
LK_PWM_DATA_t pwmHandle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  TM_DELAY_Init();

  // Initialise and start PD12 and PD14 pins/LEDs.
  // Since we are using PWM to drive PD12 LED, we set it's alternate function, that is AF2 which maps to TIM4_CH1.
  TM_GPIO_InitAlternate(GPIOD,  GPIO_PIN_12, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low, GPIO_AF2_TIM4);
  TM_GPIO_SetPinAsAlternate(GPIOD, GPIO_PIN_12);
  TM_GPIO_Init(GPIOD, GPIO_PIN_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
  TM_GPIO_SetPinHigh(GPIOD, GPIO_PIN_14);

  // Initialise the PWM signal generator.
  LK_PWM_Init(
      &pwmHandle,
      &htim4,
      TIM4,
      TIM_CHANNEL_1,
      0,
      TIM_COUNTERMODE_UP,
      8399,
      TIM_CLOCKDIVISION_DIV1,
      TIM_AUTORELOAD_PRELOAD_DISABLE,
      TIM_OCMODE_PWM2,
      TIM_OCPOLARITY_LOW,
      TIM_OCFAST_ENABLE
      );

      LK_PWM_SetPwmPercent(&pwmHandle, 95);

  while (1)
  {
    // for (int i = 0; i <= 100; i++)
    // {
    //   LK_PWM_SetPwmPercent(&pwmHandle, i);
    //   Delayms(10);
    // }
    // for (int i = 100; i >= 0; i--)
    // {
    //   LK_PWM_SetPwmPercent(&pwmHandle, i);
    //   Delayms(10);
    // }
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
