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

#define RX_TO_INVALID -1

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
void rxHandler(msg_template *msg);
void updateRx(msg_template *msg);
uint32 calcTx1(uint32 ts);
uint32 calcTx2(uint32 ts);
void writeTx2(msg_template *msg);
void configTx2(void);
void updateTx1Ts(uint32 ts);
void setRxTimeout2(void);
static void initTimerHandler(void *pContext);
static void sleepTimerHandler(void *pContext);
static void wakeTimerHandler(void *pContext);
static void activeTimerHandler(void *pContext);
static void initCycleTimings(void);
static void initListen(void);
static void initRxTo(void);
static TxStatus firstTx(uint8 mode);
static TxStatus secondTx(uint8 mode);
static void goToSleep(bool rxOn, uint32 sleep, uint32 wake);
static double calcDist(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint8 id);
static void printDists(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId);

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
uint32 dummyTime = (0x00FFFFFF / 2);
uint64 intervalDwtTime;
uint32 tx2DelayDwtTime;
uint16 rxTimeout1;
uint16 rxTimeout2;
uint32 masterRxTs;
TxStatus txStatus;
uint32 tsTable[NUM_STAMPS_PER_CYCLE][N];
// States related
bool isInitiating = false;
bool tx1Sending = false;
bool tx2Sending = false;
bool masterTx1Rdy = false;
bool waitingTx1 = false;
bool waitingTx2 = false;
// Timers related
APP_TIMER_DEF(initTimer);
APP_TIMER_DEF(sleepTimer);
APP_TIMER_DEF(activeTimer);
APP_TIMER_DEF(wakeTimer);
uint32 approxTx2; // Estimated timestamp for second TX
uint32 timeToTx1; // Duration until first transmission
uint32 timeToTx2; //  Duration until second transmission
uint64 var_delay;
uint64 reg_delay;
int counter = 0; // debugging purpose
int txCounter = 0; // debugging purpose
uint32 tx1DelayedDwtTime;
int rxPhase1To[N] = {-1};
int rxPhase2To[N];

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

static void dummyHandler(void *pContext) {
}

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
  lowTimerSingleCreate(&activeTimer, activeTimerHandler);
  lowTimerSingleCreate(&wakeTimer, wakeTimerHandler);
  lowTimerRepeatCreate(&dummyTimer, dummyHandler);
  // lowTimerStart(dummyTimer, dummyTime);

  // Pre-calculate all the timings in one cycle (ie, cycle, active, sleep period).
  initCycleTimings();

  // Initialise timestamp-related data structures.
  initTable(tsTable);

  // Initalises the messages to be transmitted.
  initTxMsgs(&txMsg1, &txMsg2);

  // Initialises the RX timeout values.
  initRxTo();

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
  // initListen();
  printf("%d\r\n", counter); // debugging purpose
  counter = 0;

  if (NODE_ID == 0)
  {
    masterTx1Rdy = true;
  }
  else
  {
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
  }
  
  // TODO: Use RX timeout to time the first and second TX.
  //       Note: RX timeout will reset to ZERO once a frame is received. 
  //       So we need to reset the timeout to a desired value each time a
  //       frame is received. Once the timeout expires, we handle it with
  //       either TX1 or TX2.
  
  while(true)
  {
    if (masterTx1Rdy)
    {
      tx1Sending = true;

      dwt_setrxtimeout(rxTimeout2); // Set BEFORE calling first TX.
      txStatus = firstTx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
      lowTimerStart(activeTimer, activePeriod);

      if (txStatus == TX_SUCCESS)
      {
        txCounter++;
      }
      else
      {
        tx1Sending = false;
      }
      masterTx1Rdy = false;
    }
  }
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
 * @brief (Synchronisation use) Calculates the tx time from u0 initial transmission.
 * 
 * @param ts the timestamp when u0 message is received.
 * @return uint32 the calculated time.
 *
 * NOTE: The chip will go into idle mode after writing the 40 bit timestamp
 *       will not able to tx or rx messages during this time.
 */
uint32 calcTx1(uint32 ts) {
  // Get current system time (higher 32 bits).
  // uint32 sys32 = dwt_readsystimestamphi32();

  // Calculate the approx. time by adding to the system time.
  uint32 approx = ts + (uint32)var_delay;
  return approx;
}

/**
 * @brief Calculates the approximated time for second transmission.
 * 
 * @param ts the timestamp after the first transmission is sent.
 * @return uint32 the calculated time.
 *
 * NOTE: The chip will go into idle mode after writing the 40 bit timestamp
 *       will not able to tx or rx messages during this time.
 */
uint32 calcTx2(uint32 ts) {
  // Get current system time (higher 32 bits).
  // uint32 sys32 = dwt_readsystimestamphi32();

  // Calculate the approx. time by adding to the system time.
  uint32 approx = ts + (uint32)reg_delay;
  return approx;
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
    memcpy((msg->data) + (i * NUM_STAMPS_PER_NODE * 4), ts, sizeof(ts));
  }
}

/**
 * @brief Configure the register parameters for first TX.
 * 
 */
void configTx1(void)
{
  uint32 delay = masterRxTs + tx1DelayedDwtTime;
  dwt_setdelayedtrxtime(delay);
  tx1Sending = true;
  firstTx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);
}

/**
 * @brief Configure the register parameters for second TX.
 * 
 * @param refTs reference timestamp to transmit the second TX from.
 */
void configTx2(void)
{
  // Set the transmission time for TX2.
  // Reference timestamp is TX1 timestamp.
  uint32 delay = tsTable[IDX_TS_1][NODE_ID] + tx2DelayDwtTime;
  dwt_setdelayedtrxtime(delay);
  delay += ((uint32)(TX_ANT_DLY)) >> 8; // TODO: Replace with Macro
  updateTable(tsTable, txMsg2, delay, NODE_ID);

  writeTx2(&txMsg2);
  txStatus = secondTx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);
  if (txStatus == TX_SUCCESS)
  {
    tx2Sending = true;
  }
  else
  {
    tx2Sending = false;
  }
}

/**
 * @brief Updates the timestamp table with first TX timestamp.
 * 
 * @param ts the first TX timestamp value (higher 32 bits).
 */
void updateTx1Ts(uint32 ts)
{
  updateTable(tsTable, txMsg1, ts, NODE_ID);
}

void setRxTimeout2(void)
{
  // Make sure device is in IDLE before changing RX timeout.
  dwt_forcetrxoff();
  dwt_setrxtimeout(rxTimeout2);
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

// TODO: Come up with an algo to update RX timeout depending on the the received ID.
void resetRxTimeout(uint8 id)
{
  uint32 timeout;

  if (waitingTx1)
  {
    timeout = rxPhase1To[id];
    dwt_forcetrxoff();
    dwt_setrxtimeout(timeout);
  }
  else if (waitingTx2)
  {
    timeout = rxPhase2To[id];
    dwt_forcetrxoff();
    dwt_setrxtimeout(timeout);
  }
  else
  {
    // Will not reach here.
  }
  
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
static void wakeTimerHandler(void *pContext)
{
  dwWake();
}

/**
 * @brief Handler function for when the waking timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void sleepTimerHandler(void *pContext)
{
  // Node 0 will transmit immediately for other nodes.
  if (NODE_ID == 0)
  {
    masterTx1Rdy = true;
  }

}

/**
 * @brief Handler function for when active period timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void activeTimerHandler(void *pContext)
{
  // printf("%d\r\n", counter);
  counter = 0;
  goToSleep(true, sleepPeriod, wakePeriod);

  // printTable(tsTable);
  printDists(tsTable, NODE_ID);
  initTable(tsTable);
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
  sleepPeriod = cyclePeriod - activePeriod;
  wakePeriod = sleepPeriod * WAKE_INIT_FACTOR;
  
  // TODO: Remove this after TzeGuang is done with TX2 ts function.
  uint64 val = TX_INTERVAL;
  uint64 conv = 65536;
  intervalDwtTime = val * conv;
  // TODO: Should not shift by 8 first, in order to preserve as much information as possible.
  tx1DelayedDwtTime = (intervalDwtTime * NODE_ID) >> 8;
  tx2DelayDwtTime = (intervalDwtTime * N) >> 8;

  // RX timeout values for synchronising TX moments.
  // The values are minused with half a interval as buffer for computation to transit to TX mode.
  rxTimeout1 = ((uint16)TX_INTERVAL * (uint16)NODE_ID) - ((uint16)TX_INTERVAL / 2);
  rxTimeout2 = ((uint16)TX_INTERVAL * N) - ((uint16)TX_INTERVAL / 2);
  
  reg_delay = (4.0 * TX_INTERVAL * 10e-6 * UUS_TO_DWT_TIME);
  reg_delay = reg_delay >> 8;
  var_delay = (NODE_ID * TX_INTERVAL * 10e-6 * UUS_TO_DWT_TIME);
  var_delay = var_delay >> 8;
  printf("cyclePeriod: %u\r\n", cyclePeriod);
  printf("activePeriod: %u\r\n", activePeriod);
  printf("wakePeriod: %u\r\n", wakePeriod);
  printf("sleepPeriod: %u\r\n", sleepPeriod);
  printf("rxTimeout1: %u\r\n", rxTimeout1);
  printf("rxTimeout2: %u\r\n", rxTimeout2);
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
    masterTx1Rdy = true;
    lowTimerStart(activeTimer, activePeriod);
    return;
  }

  isInitiating = true;

  // Start listening for one cycle
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
  lowTimerStart(initTimer, cyclePeriod);

  while (isInitiating) {};
}

/**
 * @brief Initialises the RX timeout values for each reception phase.
 * 
 */
static void initRxTo(void)
{
  int i, j;

  // Initialise the RX timeout values for Phase 1
  for (i = 0; i < N; i++)
  {
    if (i >= NODE_ID)
    {
      rxPhase1To[i] = RX_TO_INVALID;
    }
    else
    {
      rxPhase1To[i] = ((NODE_ID - i) * TX_INTERVAL) - (TX_INTERVAL / 2);
    }
  }

  // Initialise the RX timeout values for Phase 2
  i = (NODE_ID + 1 == N ? 0 : NODE_ID + 1);
  j = N - 1;
  while (i != NODE_ID)
  {
    rxPhase2To[i] = (j-- * TX_INTERVAL) - (TX_INTERVAL / 2);
    i = (i + 1 == N ? 0 : i + 1);
  }
  rxPhase2To[NODE_ID] = RX_TO_INVALID;
}

/**
 * @brief Transmits the first of the two transmissions.

 * @param mode - if 0 immediate TX (no response expected) => DWT_START_TX_IMMEDIATE
 *               if 1 delayed TX (no response expected) => DWT_START_TX_DELAYED
 *               if 2 immediate TX (response expected - so the receiver will be automatically turned on after TX is done) => DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED
 *               if 3 delayed TX (response expected - so the receiver will be automatically turned on after TX is done) => DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED
 */
static TxStatus firstTx(uint8 mode)
{
  uint8 buf[MSG_LEN];
  
  dwt_forcetrxoff();
  convertToArr(txMsg1, buf);
  
  return txMsg(buf, MSG_LEN, mode);
}

/**
 * @brief Transmits the second of the two transmissions.
 * 
 * @param nodeId Identifier of this node.
 * @param mode - if 0 immediate TX (no response expected) => DWT_START_TX_IMMEDIATE
 *               if 1 delayed TX (no response expected) => DWT_START_TX_DELAYED
 *               if 2 immediate TX (response expected - so the receiver will be automatically turned on after TX is done) => DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED
 *               if 3 delayed TX (response expected - so the receiver will be automatically turned on after TX is done) => DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED
 */
static TxStatus secondTx(uint8 mode)
{
  uint8 buf[MSG_LEN];

  dwt_forcetrxoff();
  convertToArr(txMsg2, buf);
  
  return txMsg(buf, MSG_LEN, mode);
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
  // printf("sleep\r\n\r\n"); // debugging purpose
  // dwSleep(rxOn);
  lowTimerStart(sleepTimer, sleep);
  lowTimerStart(wakeTimer, wake);
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
  goToSleep(true, sleepPeriod, cyclePeriod);
  lowTimerStop(initTimer);
  printf("sync-ing cycle by sleeping\r\n"); // debugging purpose
  isInitiating = false;
}

static uint64 get_rx_timestamp_u64(void)
{
  uint8 ts_tab[5];
  uint64 ts = 0;
  int i;
  dwt_readrxtimestamp(ts_tab);
  for (i = 4; i >= 0; i--)
  {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}

/**
 * @brief Updates the timestamps table with reception timestamp.
 * 
 * @param msg msg_template struct used to determine the location to store the timestamp.
 */
void rxHandler(msg_template *msg)
{
  uint64 ts64 = get_rx_timestamp_u64();
  uint32 ts = (uint32)(ts64 >> 8);
  updateTable(tsTable, *msg, ts, NODE_ID);

  if (msg->id == 0 && msg->isFirst == 1 && NODE_ID != 0)
  {
    // Make sure device is in IDLE first before changing RX timeout.
    dwt_forcetrxoff();
    dwt_setrxtimeout(rxTimeout1);

    lowTimerStart(activeTimer, activePeriod);
    waitingTx1 = true;
    masterRxTs = ts;
  }
}

/**
 * @brief Calculates the distance using the timestamp table.
 * 
 * @param table pointer to the 2D array representing the timestamp table.
 * @param id identifier of the node to calculate the distance with.
 * @return double the calculated distance.
 */
static double calcDist(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint8 id)
{
  double tof, num, den, timeOfFlightInUnits;
  uint64 roundTrip1, roundTrip2, replyTrip2, replyTrip1;
  uint32 ts[NUM_STAMPS_PER_CYCLE] = {0};

  // Get values to calculate.
  getFullTs(table, ts, NODE_ID, id);

  int i;
  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    if (ts[i] == 0)
    {
      return -1;
    }
  }

  // ISSUE: roundTrip1 should be >replyTrip1
  // Possibly using the wrong bits of values? (High 32 vs Low 32);
  // TODO: Test the SS_TWR example by checking the difference in TS1 and TS4. How does it compare to here?
  // POSSIBILITY: We are using the difference that is HIGHER 32 BITS and not LOWER 32 BITS.
  // So this could be that this is not the actual decimal representation. In other words,
  // We should try bit shifting to the LEFT BY 8 so we can get the full 40 bits representation.


  roundTrip1 = (ts[IDX_TS_4]) - (ts[IDX_TS_1]);
  roundTrip2 = (ts[IDX_TS_6]) - (ts[IDX_TS_3]);
  replyTrip1 = (ts[IDX_TS_3]) - (ts[IDX_TS_2]);
  replyTrip2 = (ts[IDX_TS_5]) - (ts[IDX_TS_4]);

  roundTrip1 = roundTrip1 << 8;
  roundTrip2 = roundTrip2 << 8;
  replyTrip1 = replyTrip1 << 8;
  replyTrip2 = replyTrip2 << 8;

  // printf("roundTrip1 = %lf\r\n", roundTrip1);
  // printf("roundTrip2 = %lf\r\n", roundTrip2);
  // printf("replyTrip1 = %lf\r\n", replyTrip1);
  // printf("replyTrip2 = %lf\r\n", replyTrip2);

  timeOfFlightInUnits = (double)(roundTrip1 * roundTrip2 - replyTrip1 * replyTrip2) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2);

  tof = timeOfFlightInUnits * DWT_TIME_UNITS;
  return tof * SPEED_OF_LIGHT;
}

/**
 * @brief Calculate and print all distances to serial output.
 * 
 * @param table pointer to the 2D array representing the timestamp table.
 * @param thisId identifier of the calling node.
 */
static void printDists(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId)
{
  int i;
  double dists[N] = {0};

  // Calculate the distances for each of other nodes.
  for (i = 0; i < N; i++)
  {
    if (i == thisId)
    {
      continue;
    }

    dists[i] = calcDist(table, i);
  }

  // Print the distances to serial.
  for (i = 0; i < N; i++)
  {
    if (i == thisId)
    {
      continue;
    }

    printf("%0.4lf", dists[i]);
    if ((i + 1 != N - 1 || thisId != N - 1) && i != N - 1)
    {
      printf(",");
    }
  }
  printf(";\r\n"); // Denote end of distances serial output.
}