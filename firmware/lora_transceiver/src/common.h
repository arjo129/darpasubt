// Node related
#define OP_MODE 1 // Switch between operation mode. '0' for server, '1' for client.
#define ADDRESS 1 // Unique identifier for this node (0 to 255 is allowed).

// General config
#define BAUD_RATE 115200
#define CL_WAIT_TIME 3000

// Messages related
#define MAX_DATA_LEN 30

// Frequencies config
#define CLIENT_FREQ 434.0
#define SERVER_FREQ 434.0

// Pins usage
#define PIN_TX 5 // Digital pin D5 on Arduino Uno is used.
#define PIN_RX 6 // Digital pin D6 on Arduino Uno is used.
