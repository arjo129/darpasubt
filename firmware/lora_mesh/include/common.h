#ifndef common_h
#define common_h

/* Configure address of node. Uncomment to set. */
#define MESH_ADDRESS 1
// #define MESH_ADDRESS 2
// #define MESH_ADDRESS 3
// #define MESH_ADDRESS 4

/* Configure Serial for radio comms. Uncomment to use. */
#define SERIAL_BAUD_RATE 115200
// #define USE_SOFTWARE_SERIAL
#define USE_HARDWARE_SERIAL
#ifdef USE_SOFTWARE_SERIAL
#define RX_SOFTSERIAL_PIN 10
#define TX_SOFTSERIAL_PIN 11
#endif

#ifdef USE_HARDWARE_SERIAL
#define RX_HARDSERIAL_PIN 19
#define TX_HARDSERIAL_PIN 18
#define HARD_SERIAL Serial1 // Change the Serial port if need to.
#endif

/* Network configuration values. */
#define TX_POWER 13 // 13 is default for RadioHead RF95.
#define RE_TX_RETRIES 3 // Number of retransmissions. 0 to transmit only once.
#define RE_TX_TIMEOUT 100 // Time in milliseconds to timeout retransmission.
#define MESH_ARP_TIMEOUT 100 // Time in milliseconds to timeout route resolving.
#define MAX_CHUNK_QUEUE_SIZE 20 // The size of the queue store Chunks to be sent.
#define MAX_CHUNK_ATTEMPTS 3 // Maximum number of attempts to resend a failed Chunk.

/* Define to print debugging prints. */
#define DEBUG_PRINT

/* Helper Macros for debugging prints. */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MSG_INIT_SUCCESS "Initialisation succeed"
#define MSG_INIT_FAILED "Initialisation failed"
#define MSG_WHOAMI "I am address " STR(MESH_ADDRESS)

#endif