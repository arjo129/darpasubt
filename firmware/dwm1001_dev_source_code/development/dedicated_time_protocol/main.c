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
#include "nrf_drv_rng.h"
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
#include "main.h"
#include "app_timer.h"

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
#define MAX_COUNT 1200 

#ifdef USE_FREERTOS
TaskHandle_t run_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void runTask (void * pvParameter);
TaskHandle_t led_toggle_task_handle;   /**< Reference to LED0 toggling FreeRTOS task. */
TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */
#endif

void runTask (void * pvParameter);
void printTs(uint64 val);
void printTable(uint64 table[NUM_STAMPS_PER_CYCLE][N]);
void rxHandler(msg_template *msg);
void runUser(void);
static void goToSleep(bool rxOn, uint32 sleep, uint32 wake);
static double calcDist(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 id);
static void printOutput(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId);
static void waitUser(void);
static uint64 getRandNum(void);

/*************************************************************************************************/
/********************************** INITIALISATION FUNCTIONS *************************************/
/*************************************************************************************************/
static void initCycleTimings(void);
static void initListen(void);
static void initTxMsgs(msg_template *tx1, msg_template *tx2);
static uint16 getAntDly(uint8 prf);
/*************************************************************************************************/
/******************************** INITIALISATION FUNCTIONS END ***********************************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************************** TIMER HANDLER FUNCTIONS *************************************/
/*************************************************************************************************/
static void initTimerHandler(void *pContext);
static void wakeTimerHandler(void *pContext);
static void activeTimerHandler(void *pContext);
static void rx1TimeoutHandler(void *pContext);
static void rx2TimeoutHandler(void *pContext);
static void cycleHandler(void *pContext);
static void activeInitHandler(void *pContext);
static void rx1InitHandler(void *pContext);
static void rx2InitHandler(void *pContext);
static void wakeInitHandler(void *pContext);
/*************************************************************************************************/
/********************************* TIMER HANDLER FUNCTIONS END ***********************************/
/*************************************************************************************************/

/* Global variables */
// Frames related
// msg_template is the entire frame to tx out. there is a frame format 
// (first 10 bytes and last 2 bytes) to follow, check the dw1000 manual.
msg_template txMsg1;
msg_template txMsg2;
uint16 antDelay; // Once set, must NEVER be changed.
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
bool hasSyncErr = false;
// Timers related
APP_TIMER_DEF(activeInitTimer);
APP_TIMER_DEF(wakeInitTimer);
APP_TIMER_DEF(rx1InitTimer); // First RX phase timer
APP_TIMER_DEF(rx2InitTimer); // Second RX phase timer
APP_TIMER_DEF(activeTimer);
APP_TIMER_DEF(wakeTimer);
APP_TIMER_DEF(rx1Timer); // First RX phase timer
APP_TIMER_DEF(rx2Timer); // Second RX phase timer
APP_TIMER_DEF(cycleTimer); // Repeat timer to time each cycle period.
uint64 varDelay;
uint64 regDelay;
int syncErrCount = 0; // debugging purpose
int readCount = 0;
bool waitingUser = false;

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

  // We need to read antenna delay in OTP memory with slow SPI clock.
  // We set it and NEVER change it, as it will be used for all future timestamping purpose.
  antDelay = getAntDly(DWT_PRF_64M);
  dwt_settxantennadelay(antDelay);
  dwt_setrxantennadelay(antDelay);

  // Set SPI clock to 8MHz
  port_set_dw1000_fastrate();

  // Configure DW1000.
  dwt_configure(&config);

  // Initialization of the DW1000 transceiver interrupts
  // Callback are defined in int_handler module
  dwt_setcallbacks(&tx_conf_cb, &rx_ok_cb, &rx_to_cb, &rx_err_cb);

  // Enable wanted interrupts (TX confirmation, RX good frames, RX timeouts and RX errors).
  dwt_setinterrupt(DWT_INT_TFRS | DWT_INT_RFCG | DWT_INT_RFTO | DWT_INT_RXPTO | DWT_INT_RPHE | DWT_INT_RFCE | DWT_INT_RFSL | DWT_INT_SFDT, 1);

  // Create the required timers
  lowTimerInit();
  lowTimerSingleCreate(&activeInitTimer, activeInitHandler);
  lowTimerSingleCreate(&wakeInitTimer, wakeInitHandler);
  lowTimerSingleCreate(&rx1InitTimer, rx1InitHandler);
  lowTimerSingleCreate(&rx2InitTimer, rx2InitHandler);
  lowTimerRepeatCreate(&activeTimer, activeTimerHandler);
  lowTimerRepeatCreate(&wakeTimer, wakeTimerHandler);
  lowTimerRepeatCreate(&rx1Timer, rx1TimeoutHandler);
  lowTimerRepeatCreate(&rx2Timer, rx2TimeoutHandler);
  lowTimerRepeatCreate(&cycleTimer, cycleHandler);

  // Pre-calculate all the timings in one cycle (ie, cycle, active, sleep period).
  initCycleTimings();

  // Initialise timestamp-related data structures.
  initTable(tsTable);

  // Initalises the messages to be transmitted.
  initTxMsgs(&txMsg1, &txMsg2);

  // Configure range measurement.
  if (RNG_MODE == 1)
  {
    waitUser();
    printf("Press any key to start. After 1000 measurements, press any key \
      again. Indicate a new set of 1000 measurements with \"AGAIN\".\r\n");
  }

  // Prints configuration information.
  printConfig(
    N,
    NODE_ID,
    RANGE_FREQ,
    TX_INTERVAL,
    RX_TX_BUFFER,
    cyclePeriod,
    activePeriod,
    sleepPeriod,
    WAKE_BUFFER
    );

  // Initialise the Random Number Generator module.
  nrf_drv_rng_init(NULL);

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

  // Begin the tracking of ranging cycles immediately.
  if (NODE_ID == 0)
  {
    lowTimerStart(cycleTimer, cyclePeriod);
    lowTimerStart(rx2InitTimer, rxTimeout2);
    lowTimerStart(activeInitTimer, activePeriod); // Begin to track active transceiving.
    lowTimerStart(wakeInitTimer, wakePeriod);
  }
  else
  {
    initListen();
  }
  
  
  // Application loop.
  while(true)
  {
    // Pause check for measurement mode.
    // if (readCount >= RNG_COUNT && (RNG_MODE == 1))
    // {
    //   waitUser();
    // }

    // Cycle no longer in sync with master node. Take remedial actions.
    if (hasSyncErr)
    {
      // Stop cycle timer and reset flags and transreceiver.
      lowTimerStopAll();
      dwt_forcetrxoff();
      dwt_rxreset();
      hasSyncErr = false;

      // Attempt to look for master node again.
      initListen();
    }
  }
}

/**
 *@brief Prints a 64bit unsigned timestamp value.
 *
 *@details This is for debugging purpose.
 * 
 *@param val value to be printed.
 */
void printTs(uint64 val)
{
  int i;
  uint8 ts;

  for (i = 4; i >= 0; i--)
  {
    ts = (uint8)(val >> (i * 8));
    printf("%.2x", ts);
  }
}

/**
 *@brief Prints the entire timestamps table.
 *
 *@details This is for debugging purpose.
 *
 *@param table 2D containing all the timestamps.
 */
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
 * @brief Updates first TX timestamp in timestamp table and start timer for second TX.
 * 
 * @param ts timestamp of first TX.
 */
void txUpdate(uint64 ts)
{
  updateTable(tsTable, txMsg1, ts, NODE_ID);
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
  // dwSleep(rxOn); // Commented out for development purpose.
}

/**
 * @brief Updates the timestamps table with reception timestamp and begin next phase timers.
 * 
 * @param msg msg_template struct used to determine the location to store the timestamp.
 */
void rxHandler(msg_template *msg)
{
  if (isInitiating)
  {
    isInitiating = false;

    if (msg->id == 0 && msg->isFirst == 1 && NODE_ID != 0)
    {
      lowTimerStart(cycleTimer, cyclePeriod);
      lowTimerStart(rx1InitTimer, rxTimeout1);
      lowTimerStart(rx2InitTimer, rxTimeout2);
      lowTimerStart(activeInitTimer, activePeriod);
      lowTimerStart(wakeInitTimer, wakePeriod);
    }
    return;
  }

  uint64 ts = getRxTimestampU64();
  updateTable(tsTable, *msg, ts, NODE_ID);
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
  double tof, roundTrip1, roundTrip2, replyTrip2, replyTrip1;
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

  roundTrip1 = (double)((ts[IDX_TS_4] - ts[IDX_TS_1]) * DWT_TIME_UNITS);
  roundTrip2 = (double)((ts[IDX_TS_6] - ts[IDX_TS_3]) * DWT_TIME_UNITS);
  replyTrip1 = (double)((ts[IDX_TS_3] - ts[IDX_TS_2]) * DWT_TIME_UNITS);
  replyTrip2 = (double)((ts[IDX_TS_5] - ts[IDX_TS_4]) * DWT_TIME_UNITS);

  tof = (roundTrip1 * roundTrip2 - replyTrip1 * replyTrip2) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2);

  return tof * SPEED_OF_LIGHT;
}

/**
 * @brief Prints all required output data.
 * 
 * @param table pointer to the 2D array representing the timestamp table.
 * @param thisId identifier of the calling node.
 */
static void printOutput(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint8 thisId)
{
  // Calculate the distances for each of other nodes.
  double dists[N] = {0};
  for (int i = 0; i < N; i++)
  {
    if (i == thisId)
    {
      continue;
    }

    dists[i] = calcDist(table, i);

    // Track the consecutive occurences of value errors with master node.
    if (dists[i] == -1 && i == 0)
    {
      syncErrCount++;
    }
    else if (dists[i] != -1 && i == 0)
    {
      syncErrCount = 0;
    }
  }

  // Retrieve temperature from register.
  uint16 value = dwt_readtempvbat(1); // Pass in '1' for SPI > 3MHz
  value = value >> 8; // Temperature is at the higher 8 bits.
  double temp = 1.13 * value - 113.0; // Formula to get real temperature in Celsius. See user manual.

  printData(dists, temp, thisId);
}

/**
 * @brief Stops the ranging once the desired measurement count is reached.
 *
 */
void waitUser (void)
{
  waitingUser = true;
}

/**
 * @brief Resumes the ranging after stopping range measurement.
 *
 */
void runUser (void)
{
  waitingUser = false;
  readCount = 0;
  printf("Measurements:\r\n");
}

/**
 * @brief Get a random number using the nRF Random Number Generator (RNG) module.
 * 
 * @return uint64 randomly generated 64 bit unsigned integer.
 */
static uint64 getRandNum(void)
{
  uint8 buf[8] = {0};
  uint8 bytesAvail = 0;
  uint64 ret = 0;
  int i;

  // Get the random generated bytes from the module.
  do
  {
    nrf_drv_rng_bytes_available(&bytesAvail);
  } while (bytesAvail < 8); // Wait until there are enough generated random bytes.
  
  // Read and return the random generated number.
  nrf_drv_rng_rand(buf, 8);
  for (i = 0; i < 8; i++)
  {
    ret |= buf[i];
    ret <<= 8;
  }

  return ret;
}

/*************************************************************************************************/
/********************************** INITIALISATION FUNCTIONS *************************************/
/*************************************************************************************************/
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
  wakePeriod = cyclePeriod - WAKE_BUFFER;
  
  // RX timeout values for synchronising TX moments.
  rxTimeout1 = ((uint32)TX_INTERVAL * (uint32)NODE_ID);
  rxTimeout2 = ((uint32)TX_INTERVAL * (uint32)N) + rxTimeout1;
  // The values are deducted with a fixed value to allow transition time from RX to TX for transceiver.
  rxTimeout1 -= (uint32)RX_TX_BUFFER;
  rxTimeout2 -= (uint32)RX_TX_BUFFER;
  
  interval = (uint64)TX_INTERVAL * (uint64)UUS_TO_DWT_TIME; // interval in DWT time units
  uint64 temp = 500; // TODO: We need this to allow TX2 to be configured early enough. However, this will push back the TX2 and potentially mess up with the next node attempting to receive before switching from RX to TX. Investigate.
  regDelay = (N * interval) + (temp * (uint64)UUS_TO_DWT_TIME);
  varDelay = (NODE_ID * interval);
}

/**
 * @brief Listens for master node in the network. Once master node is found, this node syncs it's cycle with master.
 *        This is handled by rxHandler() function.
 * 
 * 
 */
static void initListen(void)
{
  // Wait for random amount of time before enabling RX.
  uint64 random = getRandNum();
  uint16 time = (random % 500) + 1000; // Get a range between 10000 - 15000.
  nrf_delay_ms(time); // Delay in microseconds.
  
  isInitiating = true;

  // Start listening for one cycle
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

/**
 * @brief Initialises the TX messages to be used during the cycle.
 * 
 * @param tx1 the first TX message.
 * @param tx2 the second TX message.
 */
static void initTxMsgs(msg_template *tx1, msg_template *tx2)
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
 * @brief Retrieves the antenna delay stored in OTP memory.
 * 
 * @param prf the PRF used to configure DWM1000.
 * @return antenna delay from OTP memory.
*/
static uint16 getAntDly(uint8 prf)
{
  uint32 rawAntDelay;
  uint16 antDelay = 0;
  dwt_otpread(OTP_ANT_DLY, &rawAntDelay, 1); // '1' Refers to one 32 bits word.
  if (prf == DWT_PRF_16M)
  {
    antDelay = rawAntDelay & 0x0000FFFF; // PRF 16M Antenna delay is found in the lower 16 bits.
  }
  else if (prf == DWT_PRF_64M)
  {
    antDelay = rawAntDelay >> 16;
  }
  return antDelay;
}
/*************************************************************************************************/
/******************************** INITIALISATION FUNCTIONS END ***********************************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************************** TIMER HANDLER FUNCTIONS *************************************/
/*************************************************************************************************/
/**
 * @brief Handler function for when the sleeping timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void wakeTimerHandler(void *pContext)
{
  // printf("wake\r\n");
  // dwWake();
  if (NODE_ID != 0)
  {
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
  }
}

/**
 * @brief Handler function for when active period timer timeouts.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void activeTimerHandler(void *pContext)
{
  // printf("active\r\n");
  goToSleep(true, sleepPeriod, wakePeriod);

  // printTable(tsTable);
  printOutput(tsTable, NODE_ID);
  initTable(tsTable);
}

/**
 * @brief Handler function for when the first phase of RX is over.
 * 
 * @param pContext General parameter that can be passed into the handler.
 */
static void rx1TimeoutHandler(void *pContext)
{
  // printf("rx1\r\n");
  tx1Sending = true;
  txStatus = convertTx(&txMsg1, (DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED));
  if (txStatus == TX_SUCCESS)
  {
    // Do nothing.
  }
  else
  {
    tx1Sending = false;
  }
}

/**
 * @brief Handler function for when the second phase of RX is over.
 * 
 * @param pContext General parameter that can be passed in the handler.
 */
static void rx2TimeoutHandler(void *pContext)
{
  // printf("rx2\r\n");
  uint64 refTs = tsTable[IDX_TS_1][NODE_ID]; // First TX timestamp of this Node.
  uint64 tx2Ts = refTs + regDelay + (uint64)antDelay;
  
  updateTable(tsTable, txMsg2, tx2Ts, NODE_ID);
  writeTx2(&txMsg2, tsTable);
  tx2Sending = configTx(tsTable, regDelay, refTs, &txMsg2);
}

/**
 * @brief Handler function for tracking the start/end of each ranging cycle.
 * 
 * @param pContext General parameter that can be passed in the handler.
 */
static void cycleHandler(void *pContext)
{
  if (NODE_ID == 0)
  {
    tx1Sending = true;

    txStatus = convertTx(&txMsg1, (DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED));
    if (txStatus == TX_SUCCESS)
    {
    }
    else
    {
      tx1Sending = false;
    }
  }
  else
  {
    // Check for range value errors with master node.
    if (syncErrCount >= 10)
    {
      hasSyncErr = true;
      syncErrCount = 0;
    }
    else
    {
    }
  }
}

static void activeInitHandler(void *pContext)
{
  lowTimerStart(activeTimer, cyclePeriod);
}

static void rx1InitHandler(void *pContext)
{
  lowTimerStart(rx1Timer, cyclePeriod);
}

static void rx2InitHandler(void *pContext)
{
  lowTimerStart(rx2Timer, cyclePeriod);
}

static void wakeInitHandler(void *pContext)
{
  lowTimerStart(wakeTimer, cyclePeriod);
}
/*************************************************************************************************/
/********************************* TIMER HANDLER FUNCTIONS END ***********************************/
/*************************************************************************************************/
