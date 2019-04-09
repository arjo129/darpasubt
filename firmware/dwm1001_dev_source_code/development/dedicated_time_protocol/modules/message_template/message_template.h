#include "deca_types.h"
#include "common.h"

// Frames related
#define NUM_STAMPS_PER_NODE 3 // Number of timestamps stored that belong to each node
#define TS_LEN 5
#define ID_LEN 1 // Length of ID field
#define F_LEN 1 // Length of isFirst field
#define DATA_LEN NUM_STAMPS_PER_NODE * N * TS_LEN // Length (bytes) of data in standard message. Each timestamp is 5 bytes.
#define MSG_LEN HEADER_LEN + ID_LEN + F_LEN + DATA_LEN + CRC_LEN // Length (bytes) of the standard message
#define NUM_STAMPS_PER_CYCLE 6 // Total number of timestamps needed to compute distance
#define HEADER_LEN 10
#define CRC_LEN 2

// Position index of frame elements
#define IDX_ID 10
#define IDX_ISFIRST 11
#define IDX_DATA 12
#define IDX_CRC MSG_LEN - 2
#define IDX_TS_1 0
#define IDX_TS_2 1
#define IDX_TS_3 2
#define IDX_TS_4 3
#define IDX_TS_5 4
#define IDX_TS_6 5

/** Message template
 *
 * Note, at most 127 bytes long.
 *
 * header: usually fixed.
 * id: id of node that sent this message.
 * isFirst: whether this message is first in the cycle.
 * data: time.
 *  time: uint32, DATA_LEN timestamps.
 *    |0|0|1|1|...|i-1|i-1|i+1|i+1|...|N-1|N-1|i|i|
 *    where msg.id is i
 *      i elem: actual tx time, estimated tx time
 *      other elems: rx time, rx time
 * crc: needs to be the final two bytes.
 */
typedef struct {
  uint8 header[HEADER_LEN];
  uint8 id;
  uint8 isFirst;
  uint8 data[DATA_LEN];
  uint8 crc[CRC_LEN];
} msg_template;

/* Function prototypes */
void initTable(uint64 table[NUM_STAMPS_PER_CYCLE][N]);
void convertToStruct(uint8 *array, msg_template *msg);
void convertToArr(msg_template msg, uint8 *array);
void updateTable(uint64 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg, uint64 ts, uint8 thisId);
void getFullTs(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint64 ts[NUM_STAMPS_PER_CYCLE], uint8 thisId, uint8 targetId);
void getHalfTs(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint64 ts[NUM_STAMPS_PER_CYCLE/2], uint8 thisId, uint8 targetId);

/* Public variables */
extern uint64 tsTable[NUM_STAMPS_PER_CYCLE][N];
