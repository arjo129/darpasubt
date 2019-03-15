#include "deca_types.h"

// Frames related
#define NUM_STAMPS_PER_NODE 2 // Number of timestamps stored that belong to each node
#define DATA_LEN NUM_STAMPS_PER_NODE*(N-1)+1 // Length (bytes) of data in standard message
#define MSG_LEN 13+DATA_LEN // Length (bytes) of the standard message
#define NUM_STAMPS_PER_CYCLE 6 // Total number of timestamps needed to compute distance

// Position index of frame elements
#define IDX_ID 10
#define IDX_ISFIRST 11
#define IDX_DATA 12
#define IDX_CRC MSG_LEN - 2

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
  uint8 header[10];
  uint8 id;
  uint8 isFirst;
  uint8 data[DATA_LEN];
  uint8 crc[2];
} msg_template;

/* Function prototypes */
void initTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint8 indices[N]);
void convertToStruct(uint8 *array, msg_template *msg);
void convertToArr(msg_template msg, uint8 *array);
void updateTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg, uint32 ts);
void updateTs(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint32 ts, uint8 thisId, uint8 otherId)
void getHalfTs(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint32 ts[NUM_STAMPS_PER_CYCLE/2], uint8 id);

/* Public variables */
extern uint32 tsTable[NUM_STAMPS_PER_CYCLE][N];
extern uint8 tableIndexes[N];
