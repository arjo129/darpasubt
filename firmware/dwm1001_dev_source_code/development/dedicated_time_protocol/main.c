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
#include "nrf_drv_gpiote.h"
#include "UART.h"
#include "int_handler.h"
#include "low_timer.h"
#include "timestamper.h"
#include "message_template.h"

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

//--------------dw1000---end---------------

/* Macros definitions */
#define N 4 /**< Number of nodes */
// Antenna delays
#define TX_ANT_DLY 16456
#define RX_ANT_DLY 16456
// Ranging related
#define NODE_ID 1 // Node ID
#define RANGE_FREQ 1 // Frequency of the cycles
#define TX_INTERVAL 40000 // In microseconds
#define UUS_TO_DWT_TIME 65536 // Used to convert microseconds to DW1000 register time values.
#define SPEED_OF_LIGHT 299702547

#define TASK_DELAY 200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD 2000          /**< Timer period. LED1 timer will expire after 1000 ms */
#define TX_GAP 400 /**< Time interval between transmits, in microseconds */

#ifdef USE_FREERTOS

TaskHandle_t run_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void runTask (void * pvParameter);
TaskHandle_t led_toggle_task_handle;   /**< Reference to LED0 toggling FreeRTOS task. */
TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */
#endif

/* Local function prototypes */
void runTask (void * pvParameter);
void setTimestamps(msg_template msg);
void setTxTimestamp(uint8 *data);
void setRxTimestamp(uint8 *data);
void setTxTimestampDelayed(uint8 *data, uint32 addDelay);
msg_template getTimestamps(uint8 isFirst);
static void initTimerHandler(void *pContext);
static void sleepTimerHandler(void *pContext);
static void firstTxHandler(void *pContext);
static void secondTxHandler(void *pContext);
static void rxTimerHandler(void *pContext);
static void initCycleTimings(void);
static void initListen(void);
static void firstTx(int nodeId);
static void secondTx(int nodeId);
static void goToSleep(bool rxOn);
static double calcDist(uint8 id);

/* Global variables */
// Frames related
// msg_template is the entire frame to tx out. there is a frame format 
// (first 10 bytes and last 2 bytes) to follow, check the dw1000 manual.
uint8 msg[MSG_LEN] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint32 cyclePeriod;
uint32 activePeriod;
uint32 sleepPeriod;
TxStatus txStatus;
uint32 tsTable[NUM_STAMPS_PER_CYCLE][N];
// States related
bool isInitiating = false;
bool isSleeping = false;
// Timers related
APP_TIMER_DEF(initTimer);
APP_TIMER_DEF(sleepTimer);
APP_TIMER_DEF(tx1Timer);
APP_TIMER_DEF(tx2Timer);
APP_TIMER_DEF(rxTimer);
uint32 timeToTx1; // Duration until first transmission
uint32 timeToTx2; //  Duration until second transmission
uint32 rxTime; // Receive duration until sleep
int counter = 0; // debugging purpose
int txCounter = 0; // debugging purpose

/** Default header */
uint8 header[10] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0};

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
      // Delay a task for a given number of ticks
      vTaskDelay(TASK_DELAY);
      // Tasks must be implemented to never return...
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
#endif   // #ifdef USE_FREERTOS

int main(void)
{
  // Setup some LEDs for debug Green and Blue on DWM1001-DEV
  LEDS_CONFIGURE(BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK);
  LEDS_ON(BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK );

  #ifdef USE_FREERTOS
    // Create task for LED0 blinking with priority set to 2
    UNUSED_VARIABLE(xTaskCreate(led_toggle_task_function, "LED0", configMINIMAL_STACK_SIZE + 200, NULL, 2, &led_toggle_task_handle));

    // Start timer for LED1 blinking
    led_toggle_timer_handle = xTimerCreate( "LED1", TIMER_PERIOD, pdTRUE, NULL, led_toggle_timer_callback);
    UNUSED_VARIABLE(xTimerStart(led_toggle_timer_handle, 0));

    // Create task for SS TWR Initiator set to 2
    UNUSED_VARIABLE(xTaskCreate(runTask, "entry task", configMINIMAL_STACK_SIZE + 200, NULL, 2, &run_task_handle));
  #endif // #ifdef USE_FREERTOS
  
  //-------------dw1000  ini------------------------------------	

  // Setup NRF52832 interrupt on GPIO 25 : connected to DW1000 IRQ
  vInterruptInit();
  
  // Initialization UART
  boUART_Init();
  printf("Initialising node\r\n");
  
  // Reset DW1000
  reset_DW1000(); 

  // Set SPI clock to 2MHz
  port_set_dw1000_slowrate();			
  
  // Init the DW1000
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
  {
    //Init of DW1000 Failed
    while (1) {};
  }

  // Set SPI clock to 8MHz
  port_set_dw1000_fastrate();

  // Configure DW1000.
  dwt_configure(&config);

  // Initialization of the DW1000 transceiver interrupts
  // Callback are defined in int_handler module
  dwt_setcallbacks(&tx_conf_cb, &rx_ok_cb, &rx_to_cb, &rx_err_cb);

  // Enable wanted interrupts (TX confirmation, RX good frames, RX timeouts and RX errors).
  dwt_setinterrupt(DWT_INT_TFRS | DWT_INT_RFCG | DWT_INT_RFTO | DWT_INT_RXPTO | DWT_INT_RPHE | DWT_INT_RFCE | DWT_INT_RFSL | DWT_INT_SFDT, 1);

  // Apply default antenna delay value
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  // Create the required timers
  lowTimerInit();
  lowTimerSingleCreate(&initTimer, initTimerHandler);
  lowTimerSingleCreate(&sleepTimer, sleepTimerHandler);
  lowTimerSingleCreate(&tx1Timer, firstTxHandler);
  lowTimerSingleCreate(&tx2Timer, secondTxHandler);
  lowTimerSingleCreate(&rxTimer, rxTimerHandler);

  // Pre-calculate all the timings in one cycle (ie, cycle, active, sleep period).
  initCycleTimings();
  initTimeBuffers();

  // Initialise the timestamps table.
  initTsTable(tsTable);

  //-------------dw1000  ini------end---------------------------	
  // IF WE GET HERE THEN THE LEDS WILL BLINK

  #ifdef USE_FREERTOS		
    // Start FreeRTOS scheduler.
    vTaskStartScheduler();	

    while(1) 
    {};
  #else
    int pvParameter = 0;
    runTask(&pvParameter);
  #endif
}

/**
 * @brief Node entry function.
 * 
 * @param[in] pvParameter Pointer that will be used as the parameter for the task.
 */
void runTask (void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  dwt_setleds(DWT_LEDS_ENABLE);

  // Listen for activity in the network
  initListen();
  while (isInitiating) {};
  printf("%d\r\n", counter); // debugging purpose
  counter = 0;

  isSleeping = false;

  while(true)
  {
    counter = 0;
    while (isSleeping) {};
    isSleeping = true;

    if (NODE_ID == 0)
    {
      firstTx(NODE_ID);
      lowTimerStart(tx2Timer, timeToTx2);
    }
    else
    {
      dwt_rxenable(DWT_START_RX_IMMEDIATE);
      lowTimerStart(tx1Timer, timeToTx1);
    }
  }
}

/**
 * @brief Stores rx data in the correct buffer.
 * If rx request, store rx id, for subsequent transmission.
 *
 * Uses two global variables:
 *  timeOwn
 *  timeOthers
 *
 * @param msg - uint8[MSG_LEN] of data to be tx
 */
void setTimestamps(msg_template msg) {
  /**
   * Extract only the NODE_ID elems of time buffer.
   * Store these elems in the msg.id elems of timeOthers.
   *
   * data: time.
   *  time: uint32, DATA_LEN timestamps.
   *    |0|0|1|1|...|i-1|i-1|i+1|i+1|...|N-1|N-1|i|i|
   *    where msg.id is i
   *      i elem: actual tx time, estimated tx time
   *      other elems: rx time, rx time
   */
  if (msg.isFirst) {
    setRxTimestamp(timeOwn + NUM_STAMPS_PER_NODE*msg.id);
  } else {
    memcpy(timeOthers + NUM_STAMPS_PER_NODE*msg.id, msg.data + NUM_STAMPS_PER_NODE*NODE_ID, NUM_STAMPS_PER_NODE*sizeof(uint32));
  }
}

/**
 * @brief Reads and Stores actual tx time into data timeOwn.
 *
 * @param data - pointer to data field of msg
 *
 * Uses one global variable:
 * timeOwn
 */
void setTxTimestamp(uint8 *data) {
  uint32 time;
  time = dwt_readtxtimestamphi32();
  memcpy(data, time, sizeof(uint32));
}

/**
 * @brief Reads and Stores actual rx time into data.
 *
 * @param data - pointer to data field of msg
 */
void setRxTimestamp(uint8 *data) {
  uint32 time;
  time = dwt_readrxtimestamphi32();
  memcpy(data, time, sizeof(uint32));
}

/**
 * @brief Reads and Stores estimated tx time into data.
 *
 * @param data - pointer to data field of msg
 * @param addDelay - additional time this transmission was delayed by
 */
void setTxTimestampDelayed(uint8 *data, uint32 addDelay) {
  uint32 timeEst = dwt_read32bitoffsetreg(SYS_TIME_ID, SYS_TIME_OFFSET) + addDelay + TX_ANT_DLY;
  memcpy(data, &timeEst, sizeof(uint32));
}

/**
 * @brief Transmits three timestamps.
 *  time0: from own id.
 *
 * Uses one global variable:
 * timeOwn
 *
 * data: time.
 *  time: uint32, DATA_LEN timestamps.
 * @return msg_template
 */
msg_template getTimestamps(uint8 isFirst) {
  msg_template msg = { header, NODE_ID, isFirst };

  if (isFirst == 0) {
    uint8 data[DATA_LEN];
    memcpy(data, timeOwn, NODE_ID*NUM_STAMPS_PER_NODE*sizeof(uint32));
    memcpy(data + NODE_ID*NUM_STAMPS_PER_NODE, timeOwn + NODE_ID*NUM_STAMPS_PER_NODE, (N-NODE_ID-1)*NUM_STAMPS_PER_NODE*sizeof(uint32));
    setTxTimestampDelayed(data + (N-1)*NUM_STAMPS_PER_NODE, 0);

    // TODO put timeEst in timeOwn

    msg.data = data;
  }

  return msg;
}

/* Protocol functions */

/**
 * @brief Handler function for when the initiating timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void initTimerHandler(void *pContext)
{
  // Stop receiving frames
  dwt_forcetrxoff();
  isInitiating = false;
}

/**
 * @brief Handler function for when the sleeping timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void sleepTimerHandler(void *pContext)
{
  isSleeping = false;
  dwWake();
}

/**
 * @brief Handler function for first transmission timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void firstTxHandler(void *pContext)
{
  printf("%d\r\n", counter);
  counter = 0;
  firstTx(NODE_ID);
  setTxTimestampDelayed(timeOwn + NUM_STAMPS_PER_NODE*NODE_ID, timeToTx2);
  lowTimerStart(tx2Timer, timeToTx2);
}

/**
 * @brief Handler function for second transmission timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void secondTxHandler(void *pContext)
{
  printf("%d\r\n", counter);
  counter = 0;
  secondTx(NODE_ID);
  if (rxTimer == 0)
  {
    goToSleep(false);
  }
  else
  {
    lowTimerStart(rxTimer, rxTime);
  }
}

/**
 * @brief Handler function for reception  timer after second transmission, timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void rxTimerHandler(void *pContext)
{
  printf("%d\r\n", counter);
  counter = 0;
  goToSleep(false);
}

/**
 * @brief Initialises the cycle timings.
 * 
 * @details These timings are the cycle, active and sleep periods.
 */
static void initCycleTimings(void)
{
  cyclePeriod = 1000000 / RANGE_FREQ; // Convert from seconds to microseconds.
  activePeriod = ((2 * N - 1) * (TX_INTERVAL)); // Number of intervals in N nodes.
  // sleepPeriod = cyclePeriod - activePeriod - (NODE_ID * SLEEP_DIFF); // Staggered sleep times
  sleepPeriod = cyclePeriod - activePeriod;
  timeToTx1 = TX_INTERVAL * NODE_ID;
  timeToTx2 = N * TX_INTERVAL;
  rxTime = (N - NODE_ID - 1) * TX_INTERVAL;
}

/**
 * @brief Listens for activity in the network at initial phase.
 * 
 */
static void initListen(void)
{
  isInitiating = true;

  // Start listening for one cycle
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
  lowTimerStart(initTimer, cyclePeriod);

  // TODO: Determine how long to sleep after this listening. So the node wakes up at the correct time where protocol cycle
  // in the network begins.
}

/**
 * @brief Transmits the first of the two transmissions.
 * 
 * @param nodeId Identifier of this node.
 */
static void firstTx(int nodeId)
{
  txStatus = txMsg(msg, MSG_LEN, DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
  if (txStatus == TX_SUCCESS)
  {
    txCounter++;
    printf("1: %d\r\n", txCounter); // debugging purpose
  }
}

/**
 * @brief Transmits the second of the two transmissions.
 * 
 * @param nodeId Identifier of this node.
 */
static void secondTx(int nodeId)
{
  txStatus = txMsg(msg, MSG_LEN, DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
  if (txStatus == TX_SUCCESS)
  {
    txCounter++;
    printf("2: %d\r\n", txCounter); // debugging purpose
  }
}

/**
 * @brief Puts DW1000 to sleep.
 * 
 * @param rxOn set to true if the device should wake up with receiver on.
 */
static void goToSleep(bool rxOn)
{
  printf("sleep\r\n"); // debugging purpose
  dwSleep(rxOn);
  lowTimerStart(sleepTimer, sleepPeriod);
}

/**
 * @brief Calculates the distance using the timestamp table.
 * 
 * @return double the calculated distance.
 */
static double calcDist(uint8 id)
{
  double roundTrip1, roundTrip2, replyTrip2, replyTrip1, tof;
  uint32 ts[NUM_STAMPS_PER_CYCLE] = {0};
  int64 tof64;

  // Get values to calculate.
  getFullTs(tsTable, ts, id);

  roundTrip1 = (double)(ts[IDX_TS_4] - ts[IDX_TS_1]);
  roundTrip2 = (double)(ts[IDX_TS_6] - ts[IDX_TS_3]);
  replyTrip1 = (double)(ts[IDX_TS_3] - ts[IDX_TS_2]);
  replyTrip2 = (double)(ts[IDX_TS_5] - ts[IDX_TS_4]);

  tof64 = (int64)((roundTrip1 * roundTrip2 - replyTrip1 * replyTrip2) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2));

  tof = tof64 * DWT_TIME_UNITS;
  return tof * SPEED_OF_LIGHT;
}