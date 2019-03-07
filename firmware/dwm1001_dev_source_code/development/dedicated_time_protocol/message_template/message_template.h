// Frames related
#define NUM_STAMPS_PER_NODE 2 // Number of timestamps stored that belong to each node
#define DATA_LEN NUM_STAMPS_PER_NODE*(N-1)+1 // Length (bytes) of data in standard message
#define MSG_LEN 13+DATA_LEN // Length (bytes) of the standard message

/** Message template
 *
 * Note, at most 127 bytes long.
 *
 * header: usually fixed.
 * id: id of node that sent this message.
 * isFirst: whether this message is first in the cycle.
 * data: time.
 *  time: uint32, DATA_LEN timestamps.
 *    |0|0|1|1|...|N-1|N-1|
 *    where NODE_ID is i
 *      i elems: actual tx time, estimated transmission time
 *      other elems: rx time, rx time
 * crc: needs to be the final two bytes.
 */
typedef struct {
  uint8 header[10];
  uint8 id;
  bool isFirst;
  uint8 data[DATA_LEN];
  uint8 crc[2];
} msg_template
