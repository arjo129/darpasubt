#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"
#include "message_transceiver.h"
#include "message_processor.h"
#include "command.h"

uint8 rxBuffer[RX_BUF_LEN];
// Temporarily changed name to avoid conflicts with main.c
char sysCmdStr[MAX_CMD_SERIAL_LEN];

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn receiveInitiationMsg()
 *
 * @brief Poll for reception of initiation from Tag to begin exchange.
 *
 * @param  none
 *
 * @return the status code of reception
 */
ExecExitStatus receiveMsg(uint64 rxTimestamp, uint32 statusReg, uint8 anchorsTotalCount, uint8 *anchorsCount, uint64 *anchorsTimestamps) {
  // uint32 statusReg;
  // uint64 rxTimestamp;

  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  if (statusReg & SYS_STATUS_RXRFTO) {
    /* Clear RX timeout events before next exchange. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);

    dwt_rxreset();

    printf("=== ERROR ===\r\nRX timeout occured from listening responses. Abandoning current exchange.\r\n");
    printf("Missing Anchors:");
    int i;
    for (i = 0; i < anchorsTotalCount; i++) {
      if (anchorsTimestamps[i] == 0) {
        printf("% d |", i + 1);
      }
    }
    printf("\r\n");
    printf("*************\r\n");
    return EXEC_TIMEOUT;
  }

  if (statusReg & SYS_STATUS_RXFCG) {
    uint32 frameLen;

    /* Clear good RX frame event in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

    /* A frame has been received, read it into the local buffer. */
    frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    if (frameLen <= RX_BUF_LEN) {
      dwt_readrxdata(rxBuffer, frameLen, 0);
    }

    /* As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;

    /* Check if the frame is a command message. */
    if (memcmp(rxBuffer, sysCmdMsg, ALL_MSG_COMMON_LEN) == 0) {
      printf("Received command message\r\n");
      memcpy(sysCmdStr, &rxBuffer[SYS_CMD_IDX], MAX_CMD_SERIAL_LEN);
      return EXEC_INTERRUPTED_SYS;
    }

    /* Check that the frame is sent by an Anchor. */
    if (memcmp(rxBuffer, anchorMsg, ALL_MSG_COMMON_LEN) == 0) {
      uint8 anchorID;

      /* Retrieve poll transmission and response reception timestamps. See NOTE 5 below. */
      rxTimestamp = getRxTimestampU64();
      printf("Tag RX 1 = %u\r\n", (uint32)rxTimestamp);

      /* Retrieve the anchor number embedded in the response message. */
      anchorID = rxBuffer[ANCHOR_ID_IDX];

      /* Temporarily store the timestamps specific to the retrieved anchor number. */
      // Safety check
      if (anchorID > anchorsTotalCount) {
        printf("=== Error === Anchor number out of bounds. Anchor ID: %u\r\n", anchorID);
      } else {
        anchorsTimestamps[anchorID - 1] = rxTimestamp;
        // printf("Received Anchor ID: %u\r\n", anchorID);
        (*anchorsCount)++;
      }

      /* We need to ensure we enable RX only when we expect another response from the anchors.
        * This is crucial since enabling RX without receiving will cause any transmissions (final
        * message transmission later) to be delayed for unusually long. */
      if ((*anchorsCount) < anchorsTotalCount) {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
      }

      return EXEC_SUCCESS;
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      dwt_rxreset();

      printf("=== Error === Anchor Response Frame Incorrect\r\n");
      return EXEC_FAILURE;
    }
  }

  if (statusReg & SYS_STATUS_ALL_RX_ERR) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
    dwt_rxreset();
    printf("=== Error === Frame Received Error (Anchor Response)\r\n");
    return EXEC_FAILURE;
  }

  /* Default return value. */
  return EXEC_FAILURE;
}