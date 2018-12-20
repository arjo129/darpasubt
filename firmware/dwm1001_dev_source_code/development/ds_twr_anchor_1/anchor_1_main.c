/*! ----------------------------------------------------------------------------
*  @file    ds_resp_main.c
*  @brief   Double-sided two-way ranging (SS TWR) responder code
*
*           This application as the responder in a DS TWR distance measurement exchange. This application waits for a "poll"
*           message (recording the RX time-stamp of the poll) expected from the "DS TWR initiator" code (companion to this application), and
*           then sends a "response" message recording its TX time-stamp, after which it waits for a "final" message from the initiator to complete
*           the exchange. The final message contains the remote initiator's time-stamps of poll TX, response RX and final TX. With this data and the
*           local time-stamps, (of poll RX, response TX and final RX), this application works out a value for the time-of-flight over-the-air
*           and, thus, the estimated distance between the two devices.
*
* @attention
*
* Copyright 2018 (c) Decawave Ltd, Dublin, Ireland.
*
* All rights reserved.
*
* @author Decawave
*/
#include "sdk_config.h" 
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

/* Inter-ranging delay period, in milliseconds. See NOTE 1*/
#define RNG_DELAY_MS 80

/* Frames used in the ranging process. See NOTE 2,3 below. */
static uint8 tagFirstMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0};
static uint8 anchorMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 tagFinalMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10

/* Index to access some of the fields in the frames involved in the process. */
#define ALL_MSG_SN_IDX 2
#define FINAL_MSG_POLL_TX_TS_IDX 10
#define FINAL_MSG_FINAL_TX_TS_IDX 14
#define FINAL_MSG_RESP_RX_TS_IDX 18
#define ANCHOR_ID_IDX 10
#define FINAL_MSG_TS_LEN 4

/* ID of this anchor. */
#define ANCHORD_ID 1

/* Exchange sequence number, incremented after each transmission of the final message. */
static uint8 exchangeSeqNum = 0;

/* Buffer to store received response message.
* Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 32
static uint8 rxBuffer[RX_BUF_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
* 1 uus = 512 / 499.2 �s and 1 �s = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Delay interval between each successive anchor's reply. */
#define ACH_INTV_DLY 200000

/* Delay to turn on receiver after transmission of anchor's reply. */
#define ACH_RX_AFT_TX_DLY 1000

// Not enough time to write the data so TX timeout extended for nRF operation.
// Might be able to get away with 800 uSec but would have to test
// See note 6 at the end of this file
#define POLL_RX_TO_RESP_TX_DLY_UUS  2600

/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 500

/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 3300

/* Timestamps of frames transmission/reception.
* As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef signed long long int64;
typedef unsigned long long uint64;
static uint64 respRxTimestamp1;
static uint64 respTxTimestamp1;
static uint64 respRxTimestamp2;

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Hold copies of computed time of flight and distance here for reference so that it can be examined at a debug breakpoint. */
static double timeOfFlight;
static double distanceMetre;

/* Declaration of static functions. */
static uint64 getTxTimestampU64(void);
static uint64 getRxTimestampU64(void);
static void finalMsgGetTs(const uint8 *ts_field, uint32 *ts);

/*! ------------------------------------------------------------------------------------------------------------------
* @fn main()
*
* @brief Application entry point.
*
* @param  none
*
* @return none
*/

int ds_resp_run(void) {

  /* Clear reception timeout to start next ranging process. */
  dwt_setrxtimeout(0);

  /* Activate reception immediately. */
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
  
  printf("ID: %d\tAttempting to receive first frame...\r\n", ANCHORD_ID);
  /* Poll for reception of a frame or error/timeout. See NOTE 8 below. */
  while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  if (status_reg & SYS_STATUS_RXFCG) {
    uint32 frameLen;

    /* Clear good RX frame event in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

    /* A frame has been received, read it into the local buffer. */
    frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
    if (frameLen <= RX_BUFFER_LEN) {
      dwt_readrxdata(rxBuffer, frameLen, 0);
    }

    /* Check that the frame is a poll sent by "SS TWR initiator" example.
    * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
    rxBuffer[ALL_MSG_SN_IDX] = 0;
    if (memcmp(rxBuffer, tagFirstMsg, ALL_MSG_COMMON_LEN) == 0) {
      uint32 respSendDelayTime, anchorExtraDelay;
      int ret;

      /* Send ID of this anchor back to tag. */
      anchorMsg[ANCHOR_ID_IDX] = ANCHORD_ID;

      /* Retrieve poll reception timestamp. */
      respRxTimestamp1 = getRxTimestampU64();

      /* Set send time for response. See NOTE 9 below. */
      respSendDelayTime = (respRxTimestamp1 + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
      respSendDelayTime = respSendDelayTime + ((ANCHORD_ID - 1) * ACH_INTV_DLY);
      dwt_setdelayedtrxtime(respSendDelayTime);

      /* Set expected delay and timeout for final message reception. See NOTE 4 and 5 below. */
      uint32 rxDelay = (3 - ANCHORD_ID) * ACH_RX_AFT_TX_DLY;
      dwt_setrxaftertxdelay(rxDelay);
      // dwt_setrxtimeout(FINAL_RX_TIMEOUT_UUS);

      /* Write and send the response message. See NOTE 10 below. */
      dwt_writetxdata(sizeof(anchorMsg), anchorMsg, 0);
      dwt_writetxfctrl(sizeof(anchorMsg), 0, 1);
      ret = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);

      /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 11 below. */
      if (ret == DWT_ERROR) {
          return;
      }

      /********
       * PROBLEM: In a 2 anchors system, since the second anchor has a longer delay when sending the reply message with anchor ID, the first anchor
       * which will reply earlier with the anchor ID will go to receive mode faster. This causes the first anchor to receive the message from the second
       * anchor, preventing it from receiving the final message from the tag.
       * /
      printf("ID: %d\tAttempting to receive final frame...\r\n", ANCHORD_ID);
      /* Poll for reception of expected "final" frame or error/timeout. See NOTE 8 below. */
      while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

      if (status_reg & SYS_STATUS_RXFCG) {
        /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

        /* A frame has been received, read it into the local buffer. */
        frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
        if (frameLen <= RX_BUF_LEN) {
            dwt_readrxdata(rxBuffer, frameLen, 0);
        }

        /* Get exchange sequence number embedded in the final message. */
        exchangeSeqNum = rxBuffer[ALL_MSG_SN_IDX];

        /* Check that the frame is a final message sent by "DS TWR initiator" example.
          * As the sequence number field of the frame is not used in this example, it can be zeroed to ease the validation of the frame. */
        rxBuffer[ALL_MSG_SN_IDX] = 0;
        if (memcmp(rxBuffer, tagFinalMsg, ALL_MSG_COMMON_LEN) == 0) {
          uint32 initTxTimestamp1, initRxTimestamp1, initTxTimestamp2;
          uint32 respRxTimestamp1_32, respTxTimestamp1_32, respRxTimestamp2_32;
          double roundTrip1, roundTrip2, replyTrip2, replyTrip1;
          int64 timeOfFlightInUnits;

          /* Retrieve response transmission and final reception timestamps. */
          respTxTimestamp1 = getTxTimestampU64();
          respRxTimestamp2 = getRxTimestampU64();

          /* Get timestamps embedded in the final message. */
          finalMsgGetTs(&rxBuffer[FINAL_MSG_POLL_TX_TS_IDX], &initTxTimestamp1);
          finalMsgGetTs(&rxBuffer[FINAL_MSG_FINAL_TX_TS_IDX], &initTxTimestamp2);
          finalMsgGetTs(&rxBuffer[FINAL_MSG_RESP_RX_TS_IDX + ((ANCHORD_ID - 1) * FINAL_MSG_TS_LEN)], &initRxTimestamp1);

          /* Compute time of flight. 32-bit subtractions give correct answers even if clock has wrapped. See NOTE 12 below. */
          respRxTimestamp1_32 = (uint32)respRxTimestamp1;
          respTxTimestamp1_32 = (uint32)respTxTimestamp1;
          respRxTimestamp2_32 = (uint32)respRxTimestamp2;

          roundTrip1 = (double)(initRxTimestamp1 - initTxTimestamp1);
          roundTrip2 = (double)(respRxTimestamp2_32 - respTxTimestamp1_32);
          replyTrip2 = (double)(initTxTimestamp2 - initRxTimestamp1);
          replyTrip1 = (double)(respTxTimestamp1_32 - respRxTimestamp1_32);
          timeOfFlightInUnits = (int64)((roundTrip1 * roundTrip2 - replyTrip1 * replyTrip2) / (roundTrip1 + roundTrip2 + replyTrip1 + replyTrip2));

          timeOfFlight = timeOfFlightInUnits * DWT_TIME_UNITS;
          distanceMetre = timeOfFlight * SPEED_OF_LIGHT;

          printf("Exchange #: %u\tDistance: %lf m\r\n", exchangeSeqNum, distanceMetre);
        } else {
          /* Clear RX error/timeout events in the DW1000 status register. */
          dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

          /* Reset RX to properly reinitialise LDE operation. */
          dwt_rxreset();
        }
      }
    }
  } else {
    /* Clear RX error events in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);

    /* Reset RX to properly reinitialise LDE operation. */
    dwt_rxreset();
  }
  return(1);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn getTxTimestampU64()
 *
 * @brief Get the TX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 getTxTimestampU64(void) {
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
* @fn getRxTimestampU64()
*
* @brief Get the RX time-stamp in a 64-bit variable.
*        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
*
* @param  none
*
* @return  64-bit value of the read time-stamp.
*/
static uint64 getRxTimestampU64(void) {
  uint8 ts_tab[5];
  uint64 ts = 0;
  int i;
  dwt_readrxtimestamp(ts_tab);
  for (i = 4; i >= 0; i--) {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn finalMsgGetTs()
 *
 * @brief Read a given timestamp value from the final message. In the timestamp fields of the final message, the least
 *        significant byte is at the lower address.
 *
 * @param  ts_field  pointer on the first byte of the timestamp field to read
 *         ts  timestamp value
 *
 * @return none
 */
static void finalMsgGetTs(const uint8 *ts_field, uint32 *ts) {
    int i;
    *ts = 0;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++) {
        *ts += ts_field[i] << (i * 8);
    }
}

/**@brief SS TWR Initiator task entry function.
*
* @param[in] pvParameter   Pointer that will be used as the parameter for the task.
*/
void ds_responder_task_function (void * pvParameter) {
  UNUSED_PARAMETER(pvParameter);

  dwt_setleds(DWT_LEDS_ENABLE);

  while (true) {
    ds_resp_run();
    /* Delay a task for a given number of ticks */
    vTaskDelay(RNG_DELAY_MS);
    /* Tasks must be implemented to never return... */
  }
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 4. Delays between frames have been chosen here to ensure proper synchronisation of transmission and reception of the frames between the initiator
 *    and the responder and to ensure a correct accuracy of the computed distance. The user is referred to DecaroundTrip1nging ARM Source Code Guide for more
 *    details about the timings involved in the ranging process.
 * 5. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete final frame sent by the responder at the
 *    110k data rate used (around 3.5 ms).
 * 8. We use polled mode of operation here to keep the example as simple as possible but all status events can be used to generate interrupts. Please
 *    refer to DW1000 User Manual for more details on "interrupts". It is also to be noted that STATUS register is 5 bytes long but, as the event we
 *    use are all in the first bytes of the register, we can use the simple dwt_read32bitreg() API call to access it instead of reading the whole 5
 *    bytes.
 * 9. Timestamps and delayed transmission time are both expressed in device time units so we just have to add the desired response delay to poll RX
 *    timestamp to get response transmission time. The delayed transmission time resolution is 512 device time units which means that the lower 9 bits
 *    of the obtained value must be zeroed. This also allows to encode the 40-bit value in a 32-bit words by shifting the all-zero lower 8 bits.
 * 10. dwt_writetxdata() takes the full size of the message as a parameter but only copies (size - 2) bytes as the check-sum at the end of the frame is
 *     automatically appended by the DW1000. This means that our variable could be two bytes shorter without losing any data (but the sizeof would not
 *     work anymore then as we would still have to indicate the full length of the frame to dwt_writetxdata()).
 * 11. When running this example on the EVB1000 platform with the POLL_RX_TO_RESP_TX_DLY response delay provided, the dwt_starttx() is always
 *     successful. However, in cases where the delay is too short (or something else interrupts the code flow), then the dwt_starttx() might be issued
 *     too late for the configured start time. The code below provides an example of how to handle this condition: In this case it abandons the
 *     ranging exchange and simply goes back to awaiting another poll message. If this error handling code was not here, a late dwt_starttx() would
 *     result in the code flow getting stuck waiting subsequent RX event that will will never come. The companion "initiator" example (ex_05a) should
 *     timeout from awaiting the "response" and proceed to send another poll in due course to initiate another ranging exchange.
 * 12. The high order byte of each 40-bit time-stamps is discarded here. This is acceptable as, on each device, those time-stamps are not separated by
 *     more than 2**32 device time units (which is around 67 ms) which means that the calculation of the round-trip delays can be handled by a 32-bit
 *     subtraction.
 * 13. The user is referred to DecaroundTrip1nging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 ****************************************************************************************************************************************************/
