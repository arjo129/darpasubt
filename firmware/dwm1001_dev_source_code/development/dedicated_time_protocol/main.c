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
#define SPEED_OF_LIGHT 299702547
#define CLOSE_VAL -2.0 // Return value used when distances calculated are too low.

#define TASK_DELAY 200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD 2000          /**< Timer period. LED1 timer will expire after 1000 ms */

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
uint64 calcTx1(uint64 ts);
uint64 calcTx2(uint64 ts);
void writeTx2(msg_template *msg);
void configTx2(void);
void updateTx1Ts(uint64 ts);
void setRxTimeout2(void);
static void initTimerHandler(void *pContext);
static void sleepTimerHandler(void *pContext);
static void wakeTimerHandler(void *pContext);
static void activeTimerHandler(void *pContext);
static void rx1Handler(void *pContext);
static void rx2Handler(void *pContext);
static void initCycleTimings(void);
static void initListen(void);
static void initRxTo(void);
static TxStatus firstTx(uint8 mode);
static TxStatus secondTx(uint8 mode);
static void goToSleep(bool rxOn, uint32 sleep, uint32 wake);
static double calcDist(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 id);
static void printDists(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId);

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
uint64 tx2DelayDwtTime;
uint32 rxTimeout1;
uint32 rxTimeout2;
TxStatus txStatus;
uint64 tsTable[NUM_STAMPS_PER_CYCLE][N];
// States related
bool isInitiating = false;
bool tx1Sending = false;
bool tx2Sending = false;
bool masterTx1Rdy = false;
// Timers related
APP_TIMER_DEF(initTimer);
APP_TIMER_DEF(sleepTimer);
APP_TIMER_DEF(activeTimer);
APP_TIMER_DEF(wakeTimer);
APP_TIMER_DEF(rx1Timer); // First RX phase timer
APP_TIMER_DEF(rx2Timer); // Second RX phase timer
uint64 varDelay;
uint64 regDelay;
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
  lowTimerSingleCreate(&activeTimer, activeTimerHandler);
  lowTimerSingleCreate(&wakeTimer, wakeTimerHandler);
  lowTimerSingleCreate(&rx1Timer, rx1Handler);
  lowTimerSingleCreate(&rx2Timer, rx2Handler);

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
  
  // TODO: Use NRF timer instead of rxtimeout timer to time the delay TX.
  
  while(true)
  {
    if (masterTx1Rdy)
    {
      tx1Sending = true;

      lowTimerStart(rx2Timer, rxTimeout2);
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

void printTs(uint64 val)
{
  int i;
  uint8 ts;

  for (i = 3; i >= 0; i--)
  {
    ts = (uint8)(val >> (i * 8));
    printf("%.2x", ts);
  }
}

void printTable(uint64 table[NUM_STAMPS_PER_CYCLE][N])
{
  int i,j;
  
  printf("\r\n\n");
  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    for (j = 0; j < N; j++)
    {
      printf("|");
      printTs(table[i][j]);
      printf("|\t");
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
 * @return uint64 the calculated time.
 *
 * NOTE: The chip will go into idle mode after writing the 40 bit timestamp
 *       will not able to tx or rx messages during this time.
 */
uint64 calcTx1(uint64 ts) {
  // Calculate the approx. time by adding to the system time.
  uint64 approx = ts + varDelay;
  return approx;
}

/**
 * @brief Calculates the approximated time for second transmission.
 * 
 * @param ts the timestamp after the first transmission is sent.
 * @return uint64 the calculated time.
 *
 * NOTE: The chip will go into idle mode after writing the 40 bit timestamp
 *       will not able to tx or rx messages during this time.
 */
uint64 calcTx2(uint64 ts) {
  // Calculate the approx. time by adding to the system time.
  uint64 approx = ts + regDelay;
  return approx;
}

/**
 * @brief Writes the timestamps to be received by other nodes for the second TX.
 * 
 * @param msg the message_template struct that will be transmitted out.
 */
void writeTx2(msg_template *msg) {
  uint64 ts[NUM_STAMPS_PER_CYCLE / 2];
  int i;
  
  for (i = 0; i < N; i++)
  {
    if (i == NODE_ID)
    {
      continue;
    }

    // Retrieve values for each node and copy into data member at predefined slots.
    // TODO: Create a reusable function to copy timestamps into data member.
    getHalfTs(tsTable, ts, NODE_ID, i);
    memcpy(msg->data + (i * NUM_STAMPS_PER_NODE * TS_LEN), &ts[IDX_TS_1], TS_LEN);
    memcpy(msg->data + (i * NUM_STAMPS_PER_NODE * TS_LEN) + TS_LEN, &ts[IDX_TS_2], TS_LEN);
    memcpy(msg->data + (i * NUM_STAMPS_PER_NODE * TS_LEN) + (TS_LEN * 2), &ts[IDX_TS_3], TS_LEN);
  }
}

/**
 * @brief Configure the register parameters for first TX.
 * 
 */
void configTx1(void)
{
  uint64 firstRx = tsTable[IDX_TS_2][0]; // Retrieve the first RX timestamp from Node 0.
  uint32 delay = (firstRx + varDelay) >> 8;
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
  uint64 delay64 = tsTable[IDX_TS_1][NODE_ID] + regDelay;
  uint32 delay32 = delay64 >> 8;
  dwt_setdelayedtrxtime(delay32);
  delay64 += TX_ANT_DLY;
  updateTable(tsTable, txMsg2, delay64, NODE_ID);

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
void updateTx1Ts(uint64 ts)
{
  updateTable(tsTable, txMsg1, ts, NODE_ID);
}

void setRxTimeout2(void)
{
  // Make sure device is in IDLE before changing RX timeout.
  lowTimerStart(rx2Timer, rxTimeout2);
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
  else
  {
    // NOTE: Disable the following line if not putting the hardware to sleep.
    // dwt_rxenable(DWT_START_RX_IMMEDIATE);
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
 * @brief Handler function for when the first phase of RX is over.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void rx1Handler(void *pContext)
{
  configTx1();
}

/**
 * @brief Handler function for when the second phase of RX is over.
 * 
 * @param pContext General parameter than can be passed in the handler.
 */
static void rx2Handler(void *pContext)
{
  configTx2();
}

/**
 * @brief Initialises the cycle timings.
 * 
 * @details These timings are the cycle, active and sleep periods.
 */
static void initCycleTimings(void)
{
  uint64 interval;

  cyclePeriod = 1000000 / RANGE_FREQ; // Convert from seconds to microseconds.
  activePeriod = (2 * N * TX_INTERVAL); // Number of intervals in N nodes.
  sleepPeriod = cyclePeriod - activePeriod;
  wakePeriod = sleepPeriod * WAKE_INIT_FACTOR;
  
  // RX timeout values for synchronising TX moments.
  // The values are deducted with a fixed value to allow transition time from RX to TX for transceiver.
  rxTimeout1 = ((uint32)TX_INTERVAL * (uint32)NODE_ID) - (uint32)RX_TX_BUFFER;
  rxTimeout2 = ((uint32)TX_INTERVAL * (uint32)N) - (uint32)RX_TX_BUFFER;
  
  interval = (uint64)TX_INTERVAL * (uint64)UUS_TO_DWT_TIME; // interval in DWT time units
  regDelay = (N * interval);
  varDelay = (NODE_ID * interval);

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

/**
 * @brief Updates the timestamps table with reception timestamp.
 * 
 * @param msg msg_template struct used to determine the location to store the timestamp.
 */
void rxHandler(msg_template *msg)
{
  uint64 ts = getRxTimestampU64();
  updateTable(tsTable, *msg, ts, NODE_ID);

  if (msg->id == 0 && msg->isFirst == 1 && NODE_ID != 0)
  {
    // Make sure device is in IDLE first before changing RX timeout.
    lowTimerStart(rx1Timer, rxTimeout1);

    lowTimerStart(activeTimer, activePeriod);
  }
}

/**
 * @brief Calculates the distance using the timestamp table.
 * 
 * @param table pointer to the 2D array representing the timestamp table.
 * @param id identifier of the node to calculate the distance with.
 * @return double the calculated distance.
 */
static double calcDist(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 id)
{
  double tof, num, den, timeOfFlightInUnits;
  uint64 roundTrip1, roundTrip2, replyTrip2, replyTrip1;
  uint64 roundTripP, replyTripP;
  uint64 ts[NUM_STAMPS_PER_CYCLE] = {0};

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

  roundTrip1 = ts[IDX_TS_4] - ts[IDX_TS_1];
  roundTrip2 = ts[IDX_TS_6] - ts[IDX_TS_3];
  replyTrip1 = ts[IDX_TS_3] - ts[IDX_TS_2];
  replyTrip2 = ts[IDX_TS_5] - ts[IDX_TS_4];

  // TODO: Overflow when the product is too large to be contained in uint64. Affects computer dvalues.
  // Maybe convert the timestamps to seconds first then compute?
  roundTripP = roundTrip1 * roundTrip2;
  replyTripP = replyTrip1 * replyTrip2;
  // Check if the two device is too close in distance.
  if (roundTripP < replyTripP)
  {
    return CLOSE_VAL;
  }
  timeOfFlightInUnits = (double)(roundTripP - replyTripP) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2);

  tof = timeOfFlightInUnits * DWT_TIME_UNITS;
  return tof * SPEED_OF_LIGHT;
}

/**
 * @brief Calculate and print all distances to serial output.
 * 
 * @param table pointer to the 2D array representing the timestamp table.
 * @param thisId identifier of the calling node.
 */
static void printDists(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId)
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