#include "deca_device_api.h"
#include "port_platform.h"
#include "message_processor.h"
#include "timestamper.h"

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn writeResponseMsg()
 *
 * @brief Fill the response message with this Anchor's ID.
 *
 * @param  none
 *
 * @return none
 */
void writeResponseMsg(uint8 *anchorMsg, uint8 anchorIdNum) {
  /* Send ID of this anchor back to tag. */
  anchorMsg[ANCHOR_ID_IDX] = anchorIdNum;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn writeDistanceMsg()
 *
 * @brief Fill the distance message with this Anchor's ID and calculated distance.
 *
 * @param  none
 *
 * @return none
 */
void writeDistanceMsg(uint8 *anchorDistMsg, uint8 anchorIdNum, double distanceMetre) {
  /* Write the ID and distance to transmission message. */
  anchorDistMsg[ANCHOR_ID_IDX] = anchorIdNum;
  memcpy(&anchorDistMsg[ANCHOR_DIST_IDX], &distanceMetre, sizeof(double));
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn finalMsgGetTs()
 *
 * @brief Read a given timestamp value from the final message. In the timestamp fields of the final message, the least
 *        significant byte is at the lower address.
 *
 * @param  tsField  pointer on the first byte of the timestamp field to read
 *         ts  timestamp value
 *
 * @return none
 */
void finalMsgGetTs(const uint8 *tsField, uint32 *ts) {
    int i;
    *ts = 0;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++) {
        *ts += tsField[i] << (i * 8);
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn finalMsgSetTs()
 *
 * @brief Fill a given timestamp field in the final message with the given value. In the timestamp fields of the final
 *        message, the least significant byte is at the lower address.
 *
 * @param  tsField  pointer on the first byte of the timestamp field to fill
 *         ts  timestamp value
 *
 * @return none
 */
void finalMsgSetTs(uint8 *tsField, uint64 ts) {
    int i;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++) {
        tsField[i] = (uint8) ts;
        ts >>= 8;
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn finalMsgSetRxTs()
 *
 * @brief Fill the final message witt RX timestamp values of all Anchors. In the timestamp fields of the final message,
 *        the least significant byte is at the lower address.
 *
 * @param  tsField  pointer on the first byte of the timestamp field to fill
 *         ts timestamp value
 * 
 *         anchorsTotalCount the number of values in the timestamps array below
 * 
*          anchorsTimestamps the pointer to the first value of the anchor timestamps 
 *
 * @return none
 */
void finalMsgSetRxTs(uint8 *tsField, uint16 anchorsTotalCount, uint64 *anchorsTimestamps) {
  int i, j = 0;
  uint64 ts;
  for (i = 0; i < anchorsTotalCount; i++) {
    ts = anchorsTimestamps[i];
    // We want to continuosly write all RX timestamps which are 4 bytes each. So we start at multiples of 4.
    for (j = i * FINAL_MSG_TS_LEN; j <  (i + 1) * FINAL_MSG_TS_LEN; j++) {
      tsField[j] = (uint8) ts;
      ts >>= 8;
    }
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn writeFinalMsg()
 *
 * @brief Fill the final message with all TX/RX timestamp values and frame sequence number.
 *
 * @param  tagSendDelayTime the TX delay for transmitting the final message.
 *
 * @return none
 */
void writeFinalMsg(uint32 tagSendDelayTime, uint64 tagTxTimestamp1, uint64 anchorsTotalCount, uint8 *tagFinalMsg, uint64 *anchorsTimestamps, uint8 *exchangeSeqCount) {
  /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
  uint64 tagTxTimestamp2 = (((uint64)(tagSendDelayTime & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;
  printf("Tag TX 2 = %u\r\n", (uint32)tagTxTimestamp2);

  /* Write all the timestamps in the final message. See NOTE 11 below. */
  finalMsgSetTs(&tagFinalMsg[FINAL_MSG_TX_1_IDX], tagTxTimestamp1);
  finalMsgSetTs(&tagFinalMsg[FINAL_MSG_TX_2_IDX], tagTxTimestamp2);
  finalMsgSetRxTs(&tagFinalMsg[FINAL_MSG_RX_1_IDX], anchorsTotalCount, anchorsTimestamps);

  /* Increment frame sequence number after transmission of the final message (modulo 256). */
  exchangeSeqCount++;
  tagFinalMsg[EX_SEQ_COUNT_IDX] = exchangeSeqCount;
}