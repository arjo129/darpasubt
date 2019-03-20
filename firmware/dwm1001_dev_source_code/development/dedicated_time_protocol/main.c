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
#include "message_transceiver.h"
#include "message_template.h"
#include "common.h"

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

// Ranging related
#define UUS_TO_DWT_TIME 65536 // Used to convert microseconds to DW1000 register time values.
#define SPEED_OF_LIGHT 299702547

#define TASK_DELAY 200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD 2000          /**< Timer period. LED1 timer will expire after 1000 ms */
#define TX_GAP 400 /**< Time interval between transmits, in microseconds */
// Timestamps related
#define FIRST_TX_IDX 0
#define SECOND_TX_IDX 1

#ifdef USE_FREERTOS

TaskHandle_t run_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void runTask (void * pvParameter);
TaskHandle_t led_toggle_task_handle;   /**< Reference to LED0 toggling FreeRTOS task. */
TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */
#endif

/* Local function prototypes */
void runTask (void * pvParameter);
void initTxMsgs(msg_template *tx1, msg_template *tx2);
void syncCycle(void);
void updateRx(msg_template *msg);
uint32 calcTx2(uint32 addDelay);
void writeTx2(msg_template *msg);
void rxHandler(uint8 buffer[MSG_LEN]);
static void initTimerHandler(void *pContext);
static void sleepTimerHandler(void *pContext);
static void wakeTimerHandler(void *pContext);
static void firstTxHandler(void *pContext);
static void secondTxHandler(void *pContext);
static void rxTimerHandler(void *pContext);
static void initCycleTimings(void);
static void initListen(void);
static void firstTx(int nodeId);
static void secondTx(int nodeId);
static void goToSleep(bool rxOn, uint32 sleep, uint32 wake);
static double calcDist(uint8 id);

/* Global variables */
// Frames related
// msg_template is the entire frame to tx out. there is a frame format 
// (first 10 bytes and last 2 bytes) to follow, check the dw1000 manual.
msg_template txMsg1;
msg_template txMsg2;
uint32 cyclePeriod;
uint32 activePeriod;
uint32 sleepPeriod; // Time duration for actual hardware sleeping
uint32 wakePeriod; // Time duration from the last TX/RX to the start of next cycle
TxStatus txStatus;
uint32 tsTable[NUM_STAMPS_PER_CYCLE][N];
// States related
bool isInitiating = false;
// Timers related
APP_TIMER_DEF(initTimer);
APP_TIMER_DEF(sleepTimer);
APP_TIMER_DEF(tx1Timer);
APP_TIMER_DEF(tx2Timer);
APP_TIMER_DEF(rxTimer);
APP_TIMER_DEF(wakeTimer);
uint32 approxTx2; // Estimated timestamp for second TX
uint32 timeToTx1; // Duration until first transmission
uint32 timeToTx2; //  Duration until second transmission
uint32 rxTime; // Receive duration until sleep
int counter = 0; // debugging purpose
int txCounter = 0; // debugging purpose

/** Default header */
uint8 header[HEADER_LEN] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0};

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
  lowTimerSingleCreate(&wakeTimer, wakeTimerHandler);

  // Pre-calculate all the timings in one cycle (ie, cycle, active, sleep period).
  initCycleTimings();

  // Initialise timestamp-related data structures.
  initTable(tsTable);

  // Initalises the messages to be transmitted.
  initTxMsgs(&txMsg1, &txMsg2);

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
  printf("%d\r\n", counter); // debugging purpose
  counter = 0;

  while(true) {};
}

void printTable(uint32 table[NUM_STAMPS_PER_CYCLE][N])
{
  int i,j;
  
  printf("\r\n\n");
  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    for (j = 0; j < N; j++)
    {
      printf("|%010u|\t", table[i][j]);
    }
    printf("\r\n");
  }
  printf("\r\n");
}

/**
 * @brief Initialises the TX messages to be used during the cycle.
 * 
 * @param tx1 the first TX message.
 * @param tx2 the second TX message.
 */
void initTxMsgs(msg_template *tx1, msg_template *tx2)
{
  tx1->id = NODE_ID;
  tx1->isFirst = 1;
  memcpy(tx1->header, header, HEADER_LEN);
  memset(tx1->data, 0, DATA_LEN);
  memset(tx1->crc, 0, CRC_LEN);

  tx2->id = NODE_ID;
  tx2->isFirst = 0;
  memcpy(tx2->header, header, HEADER_LEN);
  memset(tx2->data, 0, DATA_LEN);
  memset(tx2->crc, 0, CRC_LEN);
}

/**
 * @brief Calculates the approximated time for second transmission.
 * 
 * @param addDelay the delay (in uint32) to add, given in Microseconds.
 * @return uint32 the calculated time.
 */
uint32 calcTx2(uint32 addDelay) {
  // Get current system time (higher 32 bits).
  uint32 sys32 = dwt_readsystimestamphi32();

  // Convert the delay we need into system time representation
  uint64 delay = addDelay * 128 * 499200000 / 1000000; // Divide by 1e6 to get seconds form.
  delay = delay >> 8; // Shift right by 8 to get the higher 32 bits

  // Calculate the approx. time by adding to the system time.
  uint32 approx = sys32 + delay + TX_ANT_DLY;  
}

/**
 * @brief Writes the timestamps to be received by other nodes for the second TX.
 * 
 * @param msg the message_template struct that will be transmitted out.
 */
void writeTx2(msg_template *msg) {
  uint32 ts[NUM_STAMPS_PER_CYCLE / 2];
  int i;
  
  for (i = 0; i < N; i++)
  {
    if (i == NODE_ID)
    {
      continue;
    }

    // Retrieve values for each node and copy into data member at predefined slots.
    getHalfTs(tsTable, ts, NODE_ID, i);
    memcpy((msg->data) + (i * NUM_STAMPS_PER_NODE), ts, sizeof(ts));
  }
}


/**
 * @brief Handler function when reception of a frame is successful.
 * 
 * @param buffer array containing data from the frame.
 */
void rxHandler(uint8 buffer[MSG_LEN])
{
  msg_template msg;
  uint32 rxTs;

  rxTs = dwt_readrxtimestamphi32();
  convertToStruct(buffer, &msg);
  updateTable(tsTable, msg, rxTs, NODE_ID);
}

/* Protocol functions */

/**
 * @brief Handler function for when the initiating timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void initTimerHandler(void *pContext)
{
  lowTimerStart(initTimer, cyclePeriod);
  printf("silent network\r\n"); // debugging purpose
  return;
}

/**
 * @brief Handler function for when the sleeping timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void sleepTimerHandler(void *pContext)
{
  dwWake();
}

/**
 * @brief Handler function for when the waking timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void wakeTimerHandler(void *pContext)
{
  if (timeToTx1 == 0)
  {
    firstTx(NODE_ID);
    lowTimerStart(tx2Timer, timeToTx2);

    approxTx2 = calcTx2(timeToTx2);
    updateTable(tsTable, txMsg2, approxTx2, NODE_ID);
  }
  else
  {
    lowTimerStart(tx1Timer, timeToTx1);
  }
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
  lowTimerStart(tx2Timer, timeToTx2);

  approxTx2 = calcTx2(timeToTx2);
  updateTable(tsTable, txMsg2, approxTx2, NODE_ID);
}

/**
 * @brief Handler function for second transmission timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void secondTxHandler(void *pContext)
{
  uint32 tx1 = dwt_readtxtimestamphi32();
  updateTable(tsTable, txMsg1, tx1, NODE_ID);
  writeTx2(&txMsg2);

  printf("%d\r\n", counter);
  counter = 0;
  secondTx(NODE_ID);
  lowTimerStart(rxTimer, rxTime);
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
  goToSleep(true, sleepPeriod, wakePeriod);
}

/**
 * @brief Initialises the cycle timings.
 * 
 * @details These timings are the cycle, active and sleep periods.
 */
static void initCycleTimings(void)
{
  cyclePeriod = 1000000 / RANGE_FREQ; // Convert from seconds to microseconds.
  activePeriod = (2 * N * TX_INTERVAL); // Number of intervals in N nodes.
  // sleepPeriod = cyclePeriod - activePeriod - (NODE_ID * SLEEP_DIFF); // Staggered sleep times
  wakePeriod = cyclePeriod - activePeriod;
  sleepPeriod = wakePeriod * WAKE_INIT_FACTOR;
  timeToTx1 = TX_INTERVAL * NODE_ID;
  timeToTx2 = N * TX_INTERVAL;
  rxTime = (N - NODE_ID) * TX_INTERVAL;
  printf("cyclePeriod: %u\r\n", cyclePeriod);
  printf("activePeriod: %u\r\n", activePeriod);
  printf("wakePeriod: %u\r\n", wakePeriod);
  printf("sleepPeriod: %u\r\n", sleepPeriod);
  printf("timeToTx1: %u\r\n", timeToTx1);
  printf("timeToTx2: %u\r\n", timeToTx2);
  printf("rxTime: %u\r\n", rxTime);
}

/**
 * @brief Listens for activity in the network at initial phase and joins network.
 * 
 * @details This function listens for master node's activity in the network and attempts
 *          to join the network by sync-ing it's cycle with that of master node's. This
 *          is done with syncCycle() in the RX success interrupt callback function.
 * 
 *          If this node is a master node, it joins the network immediately.
 * 
 */
static void initListen(void)
{
  if (NODE_ID == 0)
  {
    firstTx(NODE_ID);
    lowTimerStart(tx2Timer, timeToTx2);
    
    approxTx2 = calcTx2(timeToTx2); 
    updateTable(tsTable, txMsg2, approxTx2, NODE_ID);
    return;
  }

  isInitiating = true;

  // Start listening for one cycle
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
  lowTimerStart(initTimer, cyclePeriod);

  while (isInitiating) {};
}

/**
 * @brief Transmits the first of the two transmissions.
 * 
 * @param nodeId Identifier of this node.
 */
static void firstTx(int nodeId)
{
  uint8 buf[MSG_LEN];

  dwt_forcetrxoff();
  convertToArr(txMsg1, buf);

  txStatus = txMsg(buf, MSG_LEN, DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
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
  uint8 buf[MSG_LEN];

  dwt_forcetrxoff();
  convertToArr(txMsg2, buf);
  
  txStatus = txMsg(buf, MSG_LEN, DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
  if (txStatus == TX_SUCCESS)
  {
    txCounter++;
    printf("2: %d\r\n", txCounter); // debugging purpose
  }
}

/**
 * @brief Puts DW1000 to sleep.
 * 
 * @details Note: \p sleep must be lesser than \p wake. Time is needed for the
 *          transceivers to initialise before actual reception can take place.
 *          Hence, there must be adequate time allocated for the transceiver to
 *          be ready before the next cycle begins.
 * 
 * @param rxOn rxOn set to true if the device needs to wake up with receiver on.
 * @param sleep time duration for actual hardware sleeping
 * @param wake time duration to the next cycle
 */
static void goToSleep(bool rxOn, uint32 sleep, uint32 wake)
{
  printf("sleep\r\n"); // debugging purpose
  // dwSleep(rxOn);
  lowTimerStart(sleepTimer, sleep);
  lowTimerStart(wakeTimer, wake);
  printTable(tsTable);
  uint32 ts1 = tsTable[1][NODE_ID];
  uint32 ts2 = tsTable[0][NODE_ID];
  uint32 tofD = ts1 - ts2;
  double tof = ((double)tofD / (double)UUS_TO_DWT_TIME);
  printf("%lf\r\n", tof);
  // Reset the timestamps table
  initTable(tsTable);
}

/**
 * @brief Syncs this node's cycle with master node by putting the device to sleep
 *        and wake up at determined time.
 * 
 * @details This function should be called immediately once the first TX of the
 *          master node is heard.
 *          The sleep timers will ensure this node wakes up at the same time as
 *          the master node.
 * 
 */
void syncCycle(void)
{
  goToSleep(true, cyclePeriod * WAKE_INIT_FACTOR, cyclePeriod);
  lowTimerStop(initTimer);
  printf("sync-ing cycle by sleeping\r\n"); // debugging purpose
  isInitiating = false;
}

/**
 * @brief Updates the timestamps table with reception timestamp.
 * 
 * @param msg msg_template struct used to determine the location to store the timestamp.
 */
void updateRx(msg_template *msg)
{
  uint32 ts = dwt_readrxtimestamphi32();
  updateTable(tsTable, *msg, ts, NODE_ID);
}

/**
 * @brief Calculates the distance using the timestamp table.
 * 
 * @param id identifier of the node to calculate the distance with.
 * @return double the calculated distance.
 */
static double calcDist(uint8 id)
{
  double roundTrip1, roundTrip2, replyTrip2, replyTrip1, tof;
  uint32 ts[NUM_STAMPS_PER_CYCLE] = {0};
  int64 tof64;

  // Get values to calculate.
  getFullTs(tsTable, ts, NODE_ID, id);

  roundTrip1 = (double)(ts[IDX_TS_4] - ts[IDX_TS_1]);
  roundTrip2 = (double)(ts[IDX_TS_6] - ts[IDX_TS_3]);
  replyTrip1 = (double)(ts[IDX_TS_3] - ts[IDX_TS_2]);
  replyTrip2 = (double)(ts[IDX_TS_5] - ts[IDX_TS_4]);

  tof64 = (int64)((roundTrip1 * roundTrip2 - replyTrip1 * replyTrip2) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2));

  tof = tof64 * DWT_TIME_UNITS;
  return tof * SPEED_OF_LIGHT;
}
