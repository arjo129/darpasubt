// Uncomment the hard/soft serial to use.
#define USE_SOFTWARE_SERIAL
// #define USE_HARDWARE_SERIAL
#define DEBUG_PRINT

#ifdef USE_SOFTWARE_SERIAL
#define RX_SOFTSERIAL_PIN 10
#define TX_SOFTSERIAL_PIN 11
#endif

#ifdef USE_HARDWARE_SERIAL
#define RX_HARDSERIAL_PIN 0
#define TX_HARDSERIAL_PIN 1
#define HARD_SERIAL Serial
#endif

#define TOTAL_NODES 4

// Mesh has much greater memory requirements, and you may need to limit the
// max message length to prevent wierd crashes
#define RH_MESH_MAX_MESSAGE_LEN 50

// In this small artifical network of 4 nodes,
// #define MESH_ADDRESS 1
#define MESH_ADDRESS 1
// #define MESH_ADDRESS 3
// #define MESH_ADDRESS 4

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MSG_INIT_SUCCESS "Initialisation succeed"
#define MSG_INIT_FAILED "Initialisation failed"
#define MSG_WHOAMI "I am address " STR(MESH_ADDRESS)
