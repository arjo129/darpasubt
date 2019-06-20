/* Macros definitions */
#define N 4 /**< Number of nodes */
// Print enables (set to 1 to print)
#define P_DIST 1
#define P_TEMP 1
// Ranging related
#define NODE_ID 0 // Node ID
#define RANGE_FREQ 40 // Frequency of the cycles
#define TX_INTERVAL 2000 // In microseconds
#define WAKE_INIT_FACTOR 0.8 // Multiplication factor used to determine actual sleep time.
#define UUS_TO_DWT_TIME 63897
#define RX_TX_BUFFER 1800 // Buffer time (us) to transit from RX to TX.

/*
  Working Configurations (N = 4)
  
  2 active devices (0, 1):
  TX_INTERVAL = 2000
  RX_TX_BUFFER = 1800
  RANGE_FREQ = 1

  3 active devices (0, 1, 2):
  TX_INTERVAL = 2000
  RX_TX_BUFFER = 1800
  RANGE_FREQ = 1

  4 active devices (0, 1, 2, 3):
  TX_INTERVAL = 2900
  RX_TX_BUFFER = 1800
  RANGE_FREQ = 40


*/

typedef unsigned long long uint64;