/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "sdk_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp.h"
#include "boards.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_uart.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "app_error.h"
#include <string.h>
#include "port_platform.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"
#include "UART.h"
#include "command.h"
	
//-----------------dw1000----------------------------

static dwt_config_t config = {
    5,                /* Channel number. */
    DWT_PRF_64M,      /* Pulse repetition frequency. */
    DWT_PLEN_128,     /* Preamble length. Used in TX only. */
    DWT_PAC8,         /* Preamble acquisition chunk size. Used in RX only. */
    10,               /* TX preamble code. Used in TX only. */
    10,               /* RX preamble code. Used in RX only. */
    0,                /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8,       /* Data rate. */
    DWT_PHRMODE_STD,  /* PHY header mode. */
    (129 + 8 - 8)     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Preamble timeout, in multiple of PAC size. See NOTE 3 below. */
#define PRE_TIMEOUT 1000

/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 100 

/*Should be accurately calculated during calibration*/
#define TX_ANT_DLY 16456
#define RX_ANT_DLY 16456

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_SUCCESS_MS 100
/* Failure delay of 150ms is the lowest value that allows successful self recovery. */
#define RNG_DELAY_FAILURE_MS 1000
/* Stop operation delay. */
#define RNG_DELAY_STOP_MS 100
/* Receive command success. */
#define RNG_DELAY_CMD_SUCCESS_MS 50

#define EXCHANGE_INTERRUPTED 3
#define EXCHANGE_TIMEOUT 2
#define EXCHANGE_SUCCESS 1
#define EXCHANGE_FAILURE 0

//--------------dw1000---end---------------


#define TASK_DELAY        200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD      2000          /**< Timer period. LED1 timer will expire after 1000 ms */

#ifdef USE_FREERTOS

TaskHandle_t  ds_initiator_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void ds_initiator_task_function (void * pvParameter);
TaskHandle_t  led_toggle_task_handle;   /**< Reference to LED0 toggling FreeRTOS task. */
TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */
#endif

/* Buffer to receive commands from UART RX buffer. */
static struct Command command;

/* Flag to indicate if an event has happened and needs to interrupt all current activities. */
bool hasInterruptEvent;

/* Enumerations */
enum OperationMode {
  MODE_UNKNOWN,
  MODE_TAG,
  MODE_ANCHOR
};

/* States to instruct the device of the next action. */
enum DeviceState {
  STATE_STANDBY,
  STATE_RECEIVE_HOST_CMD,
  STATE_DISTRB_SYS_CMD,
  STATE_EXEC_SYS_CMD,
  STATE_RECEIVE_SYS_CMD
};

/* Device's default state. */
static enum DeviceState defaultDeviceState;

/* Indicates the state of device. */
static enum DeviceState state;

/* Function prototypes */
void ds_initiator_task_function (void * pvParameter);
void setCommand(struct Command data);
static void interpretCommand(int *operationMode, uint8 *deviceId, uint8 *anchorsTotalCount);

#ifdef USE_FREERTOS

  /**@brief LED0 task entry function.
   *
   * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
   */
  static void led_toggle_task_function (void * pvParameter)
  {
    UNUSED_PARAMETER(pvParameter);
    while (true)
    {
      LEDS_INVERT(BSP_LED_0_MASK);
      /* Delay a task for a given number of ticks */
      vTaskDelay(TASK_DELAY);
      /* Tasks must be implemented to never return... */
    }
  }

  /**@brief The function to call when the LED1 FreeRTOS timer expires.
   *
   * @param[in] pvParameter   Pointer that will be used as the parameter for the timer.
   */
  static void led_toggle_timer_callback (void * pvParameter)
  {
    UNUSED_PARAMETER(pvParameter);
    LEDS_INVERT(BSP_LED_1_MASK);
  }
#else

  extern int ss_init_run(void);

#endif   // #ifdef USE_FREERTOS

int main(void)
{
  /* Setup some LEDs for debug Green and Blue on DWM1001-DEV */
  LEDS_CONFIGURE(BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK);
  LEDS_ON(BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK );

  #ifdef USE_FREERTOS
    /* Create task for LED0 blinking with priority set to 2 */
    UNUSED_VARIABLE(xTaskCreate(led_toggle_task_function, "LED0", configMINIMAL_STACK_SIZE + 200, NULL, 2, &led_toggle_task_handle));

    /* Start timer for LED1 blinking */
    led_toggle_timer_handle = xTimerCreate( "LED1", TIMER_PERIOD, pdTRUE, NULL, led_toggle_timer_callback);
    UNUSED_VARIABLE(xTimerStart(led_toggle_timer_handle, 0));

    /* Create task for SS TWR Initiator set to 2 */
    UNUSED_VARIABLE(xTaskCreate(ds_initiator_task_function, "SSTWR_INIT", configMINIMAL_STACK_SIZE + 200, NULL, 2, &ds_initiator_task_handle));
  #endif // #ifdef USE_FREERTOS
  
  //-------------dw1000  ini------------------------------------	

  /* Setup DW1000 IRQ pin */  
  nrf_gpio_cfg_input(DW1000_IRQ, NRF_GPIO_PIN_NOPULL); 		//irq
  
  /*Initialization UART*/
  boUART_Init ();
  printf("TAG --- Double Sided Two Way Ranging\r\n");
  
  /* Reset DW1000 */
  reset_DW1000(); 

  /* Set SPI clock to 2MHz */
  port_set_dw1000_slowrate();			
  
  /* Init the DW1000 */
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
  {
    //Init of DW1000 Failed
    while (1) {};
  }

  // Set SPI to 8MHz clock
  port_set_dw1000_fastrate();

  /* Configure DW1000. */
  dwt_configure(&config);

  /* Apply default antenna delay value. See NOTE 2 below. */
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  /* Set preamble timeout for expected frames. See NOTE 3 below. */
  //dwt_setpreambledetecttimeout(0); // PRE_TIMEOUT
          
  /* Set expected response's delay and timeout. 
  * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);

  /* Set the device's default state. */
  defaultDeviceState = STATE_STANDBY;
  hasInterruptEvent = false;

  //-------------dw1000  ini------end---------------------------	
  // IF WE GET HERE THEN THE LEDS WILL BLINK

  #ifdef USE_FREERTOS		
    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();	

    while(1) 
    {};
  #else

    // No RTOS task here so just call the main loop here.
    // Loop forever responding to ranging requests.
    while (1)
    {
      ds_init_run();
    }

  #endif
}

/**@brief DS TWR Tag task entry function.
*
* @param[in] pvParameter   Pointer that will be used as the parameter for the task.
*/
void ds_initiator_task_function (void * pvParameter) {
  int result;
  /* The type of operation for this node. */
  int operationMode = MODE_TAG; // Default
  uint8 deviceId = 0, anchorsTotalCount = 0;
  state = defaultDeviceState;

  UNUSED_PARAMETER(pvParameter);

  dwt_setleds(DWT_LEDS_ENABLE);

  while (true) {
    if (state == STATE_RECEIVE_HOST_CMD) {
      interpretCommand(&operationMode, &deviceId, &anchorsTotalCount); // Set device to the next correct state
      memset(&command, 0, sizeof command); // Clear the command
      hasInterruptEvent = false; // Clear interrupt flag
      vTaskDelay(RNG_DELAY_CMD_SUCCESS_MS);
    } else if (state == STATE_STANDBY) {
      vTaskDelay(RNG_DELAY_STOP_MS);
    } else if (state == STATE_RECEIVE_SYS_CMD) {
      // poll to receive sys cmd
    } else if (state == STATE_DISTRB_SYS_CMD) {
      // send sys cmd to all nodes
    } else if (state == STATE_EXEC_SYS_CMD) {
      if (operationMode == MODE_TAG) {
        result = dsInitRun(&deviceId, &anchorsTotalCount);
      }

      if (operationMode == MODE_ANCHOR) {
        result = dsRespRun();
      }

      /* Delay a task for a given number of ticks */
      if (result == EXCHANGE_SUCCESS) {
        vTaskDelay(RNG_DELAY_SUCCESS_MS);
      } else if (result == EXCHANGE_INTERRUPTED) {
        state = STATE_RECEIVE_HOST_CMD;
        vTaskDelay(RNG_DELAY_FAILURE_MS);
      } else {
        vTaskDelay(RNG_DELAY_FAILURE_MS);
      }
    } else {

    }
  }
}

void printCommand(void) {
  printf("key = %d\r\n", command.key);
  printf("id  = %u\r\n", command.thisId);
  printf("atc = %u\r\n", command.anchorsTotalCount);
  printf("nds :\r\n");
  int i;
  for (i = 0; i < 6; i++) {
    struct NodeSwitch s = command.nodeSwitches[i];
    printf("%u %u %c\r\n", s.currentId, s.newId, s.newRole);
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn setCommand()
 *
 * @brief Set the command structure so the device can execute it when the current exchange ends.
 *
 * @param  command the Command struct to set with
 *
 * @return none
 */
void setCommand(struct Command data) {
  command = data;
  printCommand();
  state = STATE_RECEIVE_HOST_CMD;
  hasInterruptEvent = true;
  printf("received interrupt\r\n");
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn interpretCommand()
 *
 * @brief Deconstruct the received command struct and set the operation mode accordingly.
 *
 * @param  operationMode pointer to the operation mode value to change.
 *
 * @return none
 */
static void interpretCommand(int *operationMode, uint8 *deviceId, uint8 *anchorsTotalCount) {
  switch(command.key) {
    case TAG_KEY:
      *operationMode = MODE_TAG;
      *deviceId = command.thisId;
      *anchorsTotalCount = command.anchorsTotalCount;
      state = STATE_STANDBY;
      printf("Switched to Tag ID: %u\r\n", *deviceId);
      break;
    case ANCHOR_KEY:
      *operationMode = MODE_ANCHOR;
      *deviceId = command.thisId;
      *anchorsTotalCount = command.anchorsTotalCount;
      state = STATE_STANDBY;
      printf("Switched to Anchor ID: %u\r\n", *deviceId);
      break;
    case START_KEY:
      state = STATE_EXEC_SYS_CMD;
      printf("Begin ranging.\r\n");
      break;
    case STOP_KEY:
      state = STATE_STANDBY;
      printf("Stop ranging.\r\n");
      break;
    case SWITCH_KEY:
    case ADDRESS_KEY:
      *deviceId = command.thisId;
      state = STATE_STANDBY;
      printf("Set device ID: %u\r\n", *deviceId);
      break;
    default:
      // Do nothing
      state = STATE_STANDBY;
      printf("Unknown command.\r\n");
      break;
  }
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The single-sided two-way ranging scheme implemented here has to be considered carefully as the accuracy of the distance measured is highly
 *    sensitive to the clock offset error between the devices and the length of the response delay between frames. To achieve the best possible
 *    accuracy, this response delay must be kept as low as possible. In order to do so, 6.8 Mbps data rate is used in this example and the response
 *    delay between frames is defined as low as possible. The user is referred to User Manual for more details about the single-sided two-way ranging
 *    process.  NB:SEE ALSO NOTE 11.
 * 2. The sum of the values is the TX to RX antenna delay, this should be experimentally determined by a calibration process. Here we use a hard coded
 *    value (expected to be a little low so a positive error will be seen on the resultant distance estimate. For a real production application, each
 *    device should have its own antenna delay properly calibrated to get good precision when performing range measurements.
 * 3. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete response frame sent by the responder at the
 *    6.8M data rate used (around 200 s).
 * 4. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 5. The user is referred to DecaRanging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 *
 ****************************************************************************************************************************************************/



