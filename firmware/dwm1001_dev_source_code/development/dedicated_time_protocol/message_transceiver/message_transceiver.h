typedef enum {
  TX_SUCCESS,
  TX_ERROR
} TxStatus;

typedef enum {
  RX_SUCCESS,
  RX_ERROR,
  RX_TIMEOUT,
  RX_INT
} RxStatus;

typedef enum {
  MSG_TYPE_REQUEST,
  MSG_TYPE_DISTANCE,
  MSG_TYPE_TIME,
  MSG_TYPE_UNKNOWN
} MsgType;

TxStatus txMsg(uint8 *msg, int msgLen, uint8 mode);
RxStatus rxMsg(uint8 *buffer, MsgType *msgType);
