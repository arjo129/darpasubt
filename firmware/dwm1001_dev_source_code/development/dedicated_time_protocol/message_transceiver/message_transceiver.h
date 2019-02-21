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
  MSG_TYPE_COMMAND,
  MSG_TYPE_ACK,
  MSG_TYPE_RNG_INIT,
  MSG_TYPE_RNG_RESP,
  MSG_TYPE_RNG_CFM,
  MSG_TYPE_RNG_DIST,
  MSG_TYPE_UNKNOWN
} MsgType;

TxStatus txMsg(uint8 *msg, int msgLen, uint8 mode);
RxStatus rxMsg(uint8 *buffer, MsgType *msgType);