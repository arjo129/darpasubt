#define RX_BUF_LEN 32 

/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10

#define MSG_TAG_INIT_LEN 13
#define MSG_ANCHOR_RESP_LEN 13
#define MSG_TAG_FINAL_LEN 32
#define MSG_ANCHOR_DIST_LEN 21
#define MSG_SYS_CMD_LEN 32

typedef enum {
  EXEC_SUCCESS,
  EXEC_FAILURE,
  EXEC_FRAME_ERROR,
  EXEC_INTERRUPTED_HOST,
  EXEC_INTERRUPTED_SYS,
  EXEC_TIMEOUT
} ExecExitStatus;

/* Frames used in the ranging process. */
extern uint8 tagFirstMsg[MSG_TAG_INIT_LEN];
extern uint8 anchorMsg[MSG_ANCHOR_RESP_LEN];
extern uint8 tagFinalMsg[MSG_TAG_FINAL_LEN];
extern uint8 anchorDistMsg[MSG_ANCHOR_DIST_LEN];
extern uint8 sysCmdMsg[MSG_SYS_CMD_LEN];

extern bool hasInterruptEvent;

ExecExitStatus receiveMsg(uint64 rxTimestamp, uint32 statusReg, uint8 anchorsTotalCount, uint8 *anchorsCount, uint64 *anchorsTimestamps);