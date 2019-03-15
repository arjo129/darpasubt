typedef enum {
  TX_SUCCESS,
  TX_ERROR
} TxStatus;

typedef enum {
  RX_SUCCESS,
  RX_ERROR,
} RxStatus;

TxStatus txMsg(uint8 *msg, int msgLen, uint8 mode);
RxStatus rxMsg(uint8 *buffer);
