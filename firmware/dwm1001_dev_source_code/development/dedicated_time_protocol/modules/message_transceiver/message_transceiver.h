#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#ifndef MESSAGE_TEMPLATE_H
#define MESSAGE_TEMPLATE_H
#include "message_template.h"
#endif

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
void writeTx2(msg_template *msg, uint64 tsTable[NUM_STAMPS_PER_CYCLE][N]);
bool configTx(
  uint64 tsTable[NUM_STAMPS_PER_CYCLE][N],
  uint64 txDelay,
  uint64 refTs,
  msg_template *txMsg2
  );
TxStatus convertTx(msg_template *msg, uint8 mode);