#include "deca_device_api.h"
#include "port_platform.h"

#define EX_SEQ_COUNT_IDX 2
#define FINAL_MSG_TX_1_IDX 10
#define FINAL_MSG_TX_2_IDX 14
#define FINAL_MSG_RX_1_IDX 18
#define ANCHOR_ID_IDX 10
#define ANCH_COUNT_IDX 10
#define ANCHOR_DIST_IDX 11
#define SYS_CMD_IDX 10

/* Length of all timestamp values. */
#define FINAL_MSG_TS_LEN 4

void writeResponseMsg(uint8 *anchorMsg, uint8 anchorIdNum);
void writeDistanceMsg(uint8 *anchorDistMsg, uint8 anchorIdNum, double distanceMetre);
void finalMsgGetTs(const uint8 *tsField, uint32 *ts);
void finalMsgSetTs(uint8 *tsField, uint64 ts);
void finalMsgSetRxTs(uint8 *tsField, uint16 anchorsTotalCount, uint64 *anchorsTimestamps);
void writeFinalMsg(uint32 tagSendDelayTime, uint64 tagTxTimestamp1, uint64 anchorsTotalCount, uint8 *tagFinalMsg, uint64 *anchorsTimestamps, uint8 *exchangeSeqCount);
