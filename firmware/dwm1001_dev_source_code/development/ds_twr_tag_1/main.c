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

#define DEFAULT_TAG_ID 0
#define DEFAULT_ANCHOR_ID 1
#define DEFAULT_DEVICE_STATE STATE_STANDBY
#define DEFAULT_OPERATION_MODE MODE_ANCHOR
#define DEFAULT_ANCHORS_COUNT 2
#define GATEWAY_DEVICE true

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
#define RNG_DELAY_ANCHOR_SUCCESS_MS 200
#define RNG_DELAY_TAG_SUCCESS_MS 300 // Must be higher than the anchor success delay
/* Failure delay of 150ms is the lowest value that allows successful self recovery. */
#define RNG_DELAY_FAILURE_MS 1000
/* Stop operation delay. */
#define RNG_DELAY_STOP_MS 500
/* Receive command success. */
#define RNG_DELAY_CMD_SUCCESS_MS 50
/* Timeout delay. */
#define RNG_DELAY_TIMEOUT_MS 2000

#define SYS_CMD_IDX 10
#define EX_SEQ_COUNT_IDX 2

#define EXCHANGE_ANCHOR_SUCCESS 6
#define EXCHANGE_TAG_SUCCESS 5
#define EXCHANGE_SYS_CMD 4
#define EXCHANGE_INTERRUPTED 3
#define EXCHANGE_TIMEOUT 2
#define EXCHANGE_SUCCESS 1
#define EXCHANGE_FAILURE 0

#define ALL_MSG_COMMON_LEN 10
#define RX_BUF_LEN 32

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

char sysCmdString[MAX_CMD_SERIAL_LEN];

/* Enumerations */
enum OperationMode {
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


// TODO: come up with a msg format to store the sys cmds that is to be read by all other nodes
static uint8 sysCmdMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Device's default state. */
static enum DeviceState defaultDeviceState;

/* Indicates the state of device. */
static enum DeviceState state;

/* Function prototypes */
void ds_initiator_task_function (void * pvParameter);
void setCommand(struct Command data);
static void interpretCommand(enum OperationMode *operationMode, enum DeviceState *state, uint8 *deviceId, uint8 *anchorsTotalCount, bool *isGateway, struct Command *sysCommand);
static void interpretSysCommand(struct Command *command, enum DeviceState *state, enum OperationMode *operationMode, uint8 *deviceId);
static void distributeSysCmd(struct Command *sysCommand);
static int waitForSysCommand(void);
static void resetTransceiverValues(void);

/* Function prototypes related to command and interpretation. */
static void setTagDevice(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId,
    uint8 *anchorsTotalCount,
    enum DeviceState *state);
static void setAnchorDevice(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId,
    uint8 *anchorsTotalCount,
    enum DeviceState *state);
static void setNetworkSwitches(
    struct Command *command,
    enum DeviceState *state,
    enum OperationMode *operationMode,
    uint8 *deviceId);
static void setSwitchesFromCmd(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId);
static void setStartDevice(enum DeviceState *state);
static void setStartNetwork(enum DeviceState *state);
static void setStopDevice(enum DeviceState *state);
static void setStopNetwork(enum DeviceState *state);
static void setAddressDevice(struct Command *command, enum DeviceState *state, uint8 *deviceId);

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
  bool isGateway = GATEWAY_DEVICE;
  /* The type of operation for this node. */
  enum OperationMode operationMode;
  uint8 deviceId, anchorsTotalCount;

  operationMode = DEFAULT_OPERATION_MODE;
  state = DEFAULT_DEVICE_STATE;
  anchorsTotalCount = DEFAULT_ANCHORS_COUNT;
  (operationMode == MODE_TAG) ? (deviceId = DEFAULT_TAG_ID) : (deviceId = DEFAULT_ANCHOR_ID);

  UNUSED_PARAMETER(pvParameter);

  dwt_setleds(DWT_LEDS_ENABLE);

  while (true) {
    if (state == STATE_RECEIVE_HOST_CMD) {
      interpretCommand(&operationMode, &state, &deviceId, &anchorsTotalCount, &isGateway, &command); // Set device to the next correct state
      hasInterruptEvent = false; // Clear interrupt flag
      vTaskDelay(RNG_DELAY_CMD_SUCCESS_MS);
    } else if (state == STATE_STANDBY) {
      memset(&command, 0, sizeof command);
      if (!isGateway) {
        dwt_setrxtimeout(0); // Make sure we wait for system command indefinitely.
        result = waitForSysCommand();
        dwt_forcetrxoff();
        if (result == EXCHANGE_ANCHOR_SUCCESS || result == EXCHANGE_TAG_SUCCESS) {
          vTaskDelay(RNG_DELAY_ANCHOR_SUCCESS_MS);
        } else if (result == EXCHANGE_INTERRUPTED) {
          state = STATE_RECEIVE_HOST_CMD;
          vTaskDelay(RNG_DELAY_FAILURE_MS);
        } else if (result == EXCHANGE_SYS_CMD) {
          state = STATE_RECEIVE_SYS_CMD;
          vTaskDelay(RNG_DELAY_FAILURE_MS);
        } else {
          vTaskDelay(RNG_DELAY_FAILURE_MS);
        }
      }
      vTaskDelay(RNG_DELAY_STOP_MS);
    } else if (state == STATE_RECEIVE_SYS_CMD) {
      printf("Executing sys cmd\r\n");
      interpretSysCommand(&command, &state, &operationMode, &deviceId);
      hasInterruptEvent = false; // Clear interrupt flag
      if (operationMode == MODE_TAG) {
        vTaskDelay(RNG_DELAY_TIMEOUT_MS);
      }
    } else if (state == STATE_DISTRB_SYS_CMD) {
      distributeSysCmd(&command);
      printf("Executing sys cmd\r\n");
      interpretCommand(&operationMode, &state, &deviceId, &anchorsTotalCount, &isGateway, &command); // Set device to the next correct state
    } else if (state == STATE_EXEC_SYS_CMD) {
      memset(&command, 0, sizeof command);
      if (operationMode == MODE_TAG) {
        resetTransceiverValues();
        result = dsInitRun(&deviceId, &anchorsTotalCount);
      }

      if (operationMode == MODE_ANCHOR) {
        result = dsRespRun(&deviceId, &anchorsTotalCount);
      }

      /* Delay a task for a given number of ticks */
      if (result == EXCHANGE_ANCHOR_SUCCESS) {
        vTaskDelay(RNG_DELAY_ANCHOR_SUCCESS_MS);
      } else if (result == EXCHANGE_INTERRUPTED) {
        state = STATE_RECEIVE_HOST_CMD;
        vTaskDelay(RNG_DELAY_FAILURE_MS);
      } else if (result == EXCHANGE_SYS_CMD) {
        state = STATE_RECEIVE_SYS_CMD;
        vTaskDelay(RNG_DELAY_FAILURE_MS);
      } else if (result == EXCHANGE_TIMEOUT && operationMode == MODE_TAG) {
        vTaskDelay(RNG_DELAY_TIMEOUT_MS);
      } else if (result == EXCHANGE_TAG_SUCCESS) {
        vTaskDelay(RNG_DELAY_TAG_SUCCESS_MS);
      } else {
        vTaskDelay(RNG_DELAY_FAILURE_MS);
      }
    } else {

    }
  }
}

static int waitForSysCommand(void) {
  uint32 statusReg = 0;
  uint8 rxBuffer[RX_BUF_LEN];

  dwt_rxenable(DWT_START_RX_IMMEDIATE);

  // Poll for command frames
  printf("Waiting for system command...\r\n");
  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))
      && !hasInterruptEvent) {};

  if (hasInterruptEvent) {
    printf("interrupt detected\r\n");
    hasInterruptEvent = false;
    return EXCHANGE_INTERRUPTED;
  }

  if (statusReg & SYS_STATUS_ALL_RX_TO) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);
    printf("=== ERROR ===\r\nRX timeout occurred from final message. Abandoning current exchange.\r\n*************\r\n");
    return EXCHANGE_TIMEOUT;
  }

  if (statusReg & SYS_STATUS_RXFCG) {
    uint32 frameLen;

    /* Clear good RX frame event in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

    /* A frame has been received, read it into the local buffer. */
    frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
    if (frameLen <= RX_BUFFER_LEN) {
      dwt_readrxdata(rxBuffer, frameLen, 0);
    }

    /* As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;

    /* Check if the frame is a command message. */
    if (memcmp(rxBuffer, sysCmdMsg, ALL_MSG_COMMON_LEN) == 0) {
      printf("Received command message\r\n");
      int i;
      for (i = 0; i < RX_BUF_LEN; i++) {
        printf("%c", rxBuffer[i]);
      }
      printf("\r\n");
      command = constructCommand(&rxBuffer[SYS_CMD_IDX]);
      return EXCHANGE_SYS_CMD;
    }
  } else {
    /* Clear RX error events in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);

    /* Reset RX to properly reinitialise LDE operation. */
    dwt_rxreset();

    printf("=== Error === Tag Initiation Frame Incorrect\r\n");
    return EXCHANGE_FAILURE;
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
 * @fn setCommand()static void writeSysCmd(uint8 *field, uint8 *serialData) {
  int i;
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
static void interpretCommand(
    enum OperationMode *operationMode,
    enum DeviceState *state,
    uint8 *deviceId,
    uint8 *anchorsTotalCount,
    bool *isGateway,
    struct Command *command) {
  switch(command->key) {
    case TAG_KEY:
      setTagDevice(command, operationMode, deviceId, anchorsTotalCount, state);
      break;
    case ANCHOR_KEY:
      setAnchorDevice(command, operationMode, deviceId, anchorsTotalCount, state);
      break;
    case START_KEY:
      setStartDevice(state);
      break;
    case STOP_KEY:
      setStopDevice(state);
      break;
    case ADDRESS_KEY:
      setAddressDevice(command, deviceId, state);
      break;
    // The following cases will change the device state to distribute command to network.
    case START_NETWORK_KEY:
      setStartNetwork(state);
      break;
    case STOP_NETWORK_KEY:
      setStopNetwork(state);
      break;
    case SWITCH_KEY:
      if (isGateway) {
        setNetworkSwitches(command, state, operationMode, deviceId);
      }
      break;
    default:
      // Do nothing
      printf("Unknown command.\r\n");
      break;
  }
}

static void interpretSysCommand(struct Command *command, enum DeviceState *state, enum OperationMode *operationMode, uint8 *deviceId) {
  switch(command->key) {
    case START_NETWORK_KEY:
      *state = STATE_EXEC_SYS_CMD;
      break;
    case STOP_NETWORK_KEY:
      *state = STATE_STANDBY;
      break;
    case SWITCH_KEY:
      printf("Switching roles\r\n");
      printCommand();
      setSwitchesFromCmd(command, operationMode, deviceId);
      *state = STATE_STANDBY;
      break;
    default:
      printf("Unknown system command\r\n");
  }
}

static void writeSysCmd(uint8 *field, uint8 *serialData) {
  int i;
  for (i = 0; i < MAX_CMD_SERIAL_LEN; i++) {
    field[i] = serialData[i];
  }
}

static void distributeSysCmd(struct Command *sysCommand) {
  int ret;
  uint8 serialData[MAX_CMD_SERIAL_LEN];

  serializeCommand(*sysCommand, serialData);
  printf("sending ");
  int i;
  for (i = 0; i < MAX_CMD_SERIAL_LEN; i++) {
    printf(" %c", serialData[i]);
  }
  printf("\r\n");
  writeSysCmd(&sysCmdMsg[SYS_CMD_IDX], serialData);

  /* Write data to TX buffer and prepare for transmission. */
  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
  dwt_writetxdata(sizeof(sysCmdMsg), sysCmdMsg, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(sizeof(sysCmdMsg), 0, 1); /* Zero offset in TX buffer, ranging. */

  /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent. */
  ret = dwt_starttx(DWT_START_TX_IMMEDIATE);
  if (ret == DWT_SUCCESS) {
    /* Ensure transmission occurs. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};
    dwt_forcetrxoff();
    printf("Distribute command success\r\n");
  } else {
    printf("Distribute command failure\r\n");
  }
}

static void setTagDevice(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId,
    uint8 *anchorsTotalCount,
    enum DeviceState *state) {
  *operationMode = MODE_TAG;
  *deviceId = command->thisId;
  *anchorsTotalCount = command->anchorsTotalCount;
  *state = STATE_STANDBY;
  printf("Switched to Tag ID: %u\r\n", *deviceId);
}

static void setAnchorDevice(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId,
    uint8 *anchorsTotalCount,
    enum DeviceState *state) {
  *operationMode = MODE_ANCHOR;
  *deviceId = command->thisId;
  *anchorsTotalCount = command->anchorsTotalCount;
  *state = STATE_STANDBY;
  printf("Switched to Anchor ID: %u\r\n", *deviceId);
}

static void setStartDevice(enum DeviceState *state) {
  *state = STATE_EXEC_SYS_CMD;
  printf("Begin ranging for this device.\r\n");
}

static void setStartNetwork(enum DeviceState *state) {
  if (*state == STATE_RECEIVE_HOST_CMD) {
    *state = STATE_DISTRB_SYS_CMD;
    printf("Begin network ranging.\r\n");
    return;
  }
  if (*state == STATE_RECEIVE_SYS_CMD) {
    *state = STATE_EXEC_SYS_CMD;
    printf("Begin network ranging with system command.\r\n");
    return;
  }
  if (*state == STATE_DISTRB_SYS_CMD) {
    *state = STATE_EXEC_SYS_CMD;
    printf("Begin ranging for this device.\r\n");
    return;
  }
}

static void setStopDevice(enum DeviceState *state) {
  *state = STATE_STANDBY;
  printf("Stop ranging for this device.\r\n");
}

static void setStopNetwork(enum DeviceState *state) {
  if (*state == STATE_RECEIVE_HOST_CMD) {
    *state = STATE_DISTRB_SYS_CMD;
    printf("Stop network ranging.\r\n");
    return;
  }
  if (*state == STATE_RECEIVE_SYS_CMD) {
    *state = STATE_STANDBY;
    printf("Stop network ranging with system command.\r\n");
    return;
  }
  if (*state == STATE_DISTRB_SYS_CMD) {
    *state = STATE_STANDBY;
    printf("Stop ranging for this device.\r\n");
    return;
  }
}

static void setNetworkSwitches(
    struct Command *command,
    enum DeviceState *state,
    enum OperationMode *operationMode,
    uint8 *deviceId) {
  if (*state == STATE_RECEIVE_HOST_CMD) {
    *state = STATE_DISTRB_SYS_CMD;
    return;
  }
  if (*state == STATE_DISTRB_SYS_CMD) {
    setSwitchesFromCmd(command, operationMode, deviceId);
    *state = STATE_STANDBY;
    return;
  }
  printf("Switching network nodes.\r\n");
}

static void setAddressDevice(struct Command *command, enum DeviceState *state, uint8 *deviceId) {
  *deviceId = command->thisId;
  *state = STATE_STANDBY;
  printf("Set new address ID: %u\r\n", *deviceId);
}

static void setSwitchesFromCmd(
    struct Command *command,
    enum OperationMode *operationMode,
    uint8 *deviceId) {
  int numberOfSwitches = command->numberOfSwitches;
  int i;

  for (i = 0; i < numberOfSwitches; i++) {
    struct NodeSwitch currSwitch = command->nodeSwitches[i];
    if (currSwitch.currentId == *deviceId) {
      printf("id matches: %d\r\n", *deviceId);
      if (currSwitch.newRole == TAG_CHAR) {
        *operationMode = MODE_TAG;
        printf("new mode: tag\r\n");
      } else if (currSwitch.newRole == ANCHOR_CHAR) {
        *operationMode = MODE_ANCHOR;
        printf("new mode: anchor\r\n");
      } else {
        printf("continue\r\n");
        continue;
      }
      *deviceId = currSwitch.newId;
      printf("new id: %d\r\n", *deviceId);
      break;
    }
  }
}

static void resetTransceiverValues(void) {
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(0);
  dwt_forcetrxoff();
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



