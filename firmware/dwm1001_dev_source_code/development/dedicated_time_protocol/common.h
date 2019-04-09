/* Macros definitions */
#define N 4 /**< Number of nodes */
// Antenna delays
#define TX_ANT_DLY 16456
#define RX_ANT_DLY 16456
// Ranging related
#define NODE_ID 1 // Node ID
#define RANGE_FREQ 1 // Frequency of the cycles
#define TX_INTERVAL 10000 // In microseconds
#define WAKE_INIT_FACTOR 0.8 // Multiplication factor used to determine actual sleep time.
#define UUS_TO_DWT_TIME 63897
#define RX_TX_BUFFER 2000 // Buffer time (us) to transit from RX to TX.

typedef unsigned long long uint64;