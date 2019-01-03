/*! ----------------------------------------------------------------------------
*  @file    ds_init_main.c
*  @brief   Double-sided two-way ranging (DS TWR) initiator code
*
*           This application acts as the initiator in a DS TWR distance measurement exchange. This application sends a "poll"
*           frame (recording the TX time-stamp of the poll), after which it waits for a "response" message from the "DS TWR responder"
*           code (companion to this application) to complete the exchange. When the response message is received, we record the RX-timestamp of
*           this reception and send a final message to complete this exchange.
*           This final message contains the timestamps recorded by the device running this application. The companion "DS TWR responder"
*           application then works out the time of flight over the air and thus, the estimated distance.
* 
* @attention
*
* Copyright 2015 (c) Decawave Ltd, Dublin, Ireland.
*
* All rights reserved.
*
* @author Decawave
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"
#include "UART.h"

#define APP_NAME "DS TWR TAG"

/* Total number of anchors to participate in the exchange. 
* Minimum of 1 and maximum of 256, ie. a 1-byte value. */
////////////////////*** IMPORTANT ***/////////////////////
///// ENSURE THIS IS CORRECTLY SET BEFORE OPERATION! /////
#define ANCHORS_TOTAL_COUNT 3

/* Length of the common part of the message (up to and including the function code, see NOTE 1 below). */
#define ALL_MSG_COMMON_LEN 10

/* Indexes to access some of the fields in the frames. */
#define EX_SEQ_COUNT_IDX 2
#define ANCH_COUNT_IDX 10
#define FINAL_MSG_TX_1_IDX 10
#define FINAL_MSG_TX_2_IDX 14
#define FINAL_MSG_RX_1_IDX 18
#define ANCHOR_ID_IDX 10
#define ANCHOR_DIST_IDX 11

/* Values to help determine if message transmitted or received. */
#define INITIATION_SEND_SUCCESS 1
#define INITIATION_SEND_FAILURE 0
#define ANCHOR_RECEIVE_TIMEOUT 2
#define ANCHOR_RECEIVE_SUCCESS 1
#define ANCHOR_RECEIVE_FAILURE 0
#define FINAL_SEND_SUCCESS 1
#define FINAL_SEND_FAILURE 0
#define DISTANCE_RECEIVE_TIMEOUT 2
#define DISTANCE_RECEIVE_SUCCESS 1
#define DISTANCE_RECEIVE_FAILURE 0
#define EXCHANGE_TIMEOUT 2
#define EXCHANGE_SUCCESS 1
#define EXCHANGE_FAILURE 0

/* Length of buffer to store received messages. */
#define RX_BUF_LEN 32

/* Length of all timestamp values. */
#define FINAL_MSG_TS_LEN 4

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
* 1 uus = 512 / 499.2 �s and 1 �s = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3800

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Frames used in the ranging process. See NOTE 2 below. */
static uint8 tagFirstMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0};
static uint8 anchorMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0};
static uint8 tagFinalMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 anchorDistMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Buffer to store received response message.
* Its size is adjusted to longest frame that this application is supposed to handle. */
static uint8 rxBuffer[RX_BUF_LEN];

/* Temporary storage for the timestamps to be sent to anchors. */
static uint64 anchorsTimestamps[ANCHORS_TOTAL_COUNT];

/* Temporary storage for the distances. */
static double anchorsDistances[ANCHORS_TOTAL_COUNT];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 statusReg = 0;

/* Time-stamps of frames transmission/reception, expressed in device time units.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef unsigned long long uint64;
static uint64 tagTxTimestamp1;
static uint64 tagRxTimestamp1;
static uint64 tagTxTimestamp2;

/*Transactions Counters */
static volatile int txCount = 0 ; // Successful transmit counter
static volatile int rxCount = 0 ; // Successful receive counter 
static volatile int anchorsCount = 0; // Counter for response from anchors

/* Exchange sequence number, incremented after each transmission of the final message. */
static uint8 exchangeSeqCount = 0;

/* Flag to determine if message has been transmitted or received. */
static int sendInitiation = 0;
static int anchorReceive = 0;
static int finalSend = 0;
static int distanceReceive = 0;

/* Declaration of static functions. */
static uint64 getTxTimestampU64(void);
static uint64 getRxTimestampU64(void);
static void finalMsgSetTs(uint8 *tsField, uint64 ts);
static void finalMsgSetRxTs(uint8 *tsField);
static uint32 setFinalTxDelay(void);
static void writeFinalMsg(uint32 tagSendDelayTime);
static int sendInitiationMsg(void);
static int sendFinalMsg(void);
static int receiveAnchorResponse(void);
static int receiveDistanceMsgs(void);
static void printDistance(void);

/* TODO:
 * 2. Possibly use soft reset to restart clocks to reduce clock drift maybe?
 * 4. Add proper comments NOTES to document this code.
 * /

/*! ------------------------------------------------------------------------------------------------------------------
* @fn main()
*
* @brief Application entry point.
*
* @param  none
*
* @return none
*/
int dsInitRun(void) {
  /* Loop forever initiating ranging exchanges. */
  uint32 tagSendDelayTime;

  /* Clears the anchor timestamps temporary storage. */
  memset(anchorsTimestamps, 0, sizeof anchorsTimestamps);
  memset(anchorsDistances, 0, sizeof anchorsDistances);

  /* Notifies initiating of measurement exchange. */
  printf("Initiating Exchange #%u\r\n", exchangeSeqCount + 1);
  sendInitiation = sendInitiationMsg();
  if (sendInitiation == INITIATION_SEND_FAILURE) {
    return EXCHANGE_FAILURE;
  }

  /* Poll for reception of frames from all anchors, loop until response from all anchors have been received. */
  anchorsCount = 0;
  while (anchorsCount < ANCHORS_TOTAL_COUNT) {
    anchorReceive = receiveAnchorResponse();
    if (anchorReceive == ANCHOR_RECEIVE_TIMEOUT) {
      return EXCHANGE_TIMEOUT;
    }
    if (anchorReceive == ANCHOR_RECEIVE_FAILURE) {
      return EXCHANGE_FAILURE;
    }
  }

  /* Received every anchor's response, now we notify them with a final confirmation message. */
  tagSendDelayTime = setFinalTxDelay();
  writeFinalMsg(tagSendDelayTime);
  finalSend = sendFinalMsg();
  if (finalSend == FINAL_SEND_FAILURE) {
    return EXCHANGE_FAILURE;
  }
  
  /* For for reception of the distances. */
  anchorsCount = 0;
  while (anchorsCount < ANCHORS_TOTAL_COUNT) {
    distanceReceive = receiveDistanceMsgs();
    if (distanceReceive == DISTANCE_RECEIVE_TIMEOUT) {
      return EXCHANGE_TIMEOUT;
    }
    if (distanceReceive == DISTANCE_RECEIVE_FAILURE) {
      return EXCHANGE_FAILURE;
    }
  }

  printDistance();

  /* Execute a delay between ranging exchanges. */
  // deca_sleep(RNG_DELAY_SUCCESS_MS);

  return EXCHANGE_SUCCESS;
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
    uint8 tsTab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(tsTab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= tsTab[i];
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
    uint8 tsTab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(tsTab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= tsTab[i];
    }
    return ts;
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
 * @return none
 */
static void finalMsgSetRxTs(uint8 *tsField) {
  int i, j = 0;
  uint64 ts;
  for (i = 0; i < ANCHORS_TOTAL_COUNT; i++) {
    ts = anchorsTimestamps[i];
    // We want to continuosly write all RX timestamps which are 4 bytes each. So we start at multiples of 4.
    for (j = i * FINAL_MSG_TS_LEN; j <  (i + 1) * FINAL_MSG_TS_LEN; j++) {
      tsField[j] = (uint8) ts;
      ts >>= 8;
    }
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn printDistance()
 *
 * @brief Prints all the distances gathered from every Anchors in the system.
 *
 * @param  none
 *
 * @return none
 */
static void printDistance(void) {
  int i = 0;
  double dist;
  for (int i = 0; i < ANCHORS_TOTAL_COUNT; i++) {
    printf("%lf", anchorsDistances[i]);
    if (i < ANCHORS_TOTAL_COUNT - 1) {
      printf(",");
    }
  }
  printf(";\r\n");
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
static void finalMsgSetTs(uint8 *tsField, uint64 ts) {
    int i;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++) {
        tsField[i] = (uint8) ts;
        ts >>= 8;
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn setFinalTxDelay()
 *
 * @brief Set the TX delay for sending the final message. The calculated value is based on the last anchor ID.
 *
 * @param  none
 *
 * @return none
 */
static uint32 setFinalTxDelay(void) {
  uint32 tagSendDelayTime;

  /* Compute final message transmission time. See NOTE 10 below. */
  // Uses the RX timestamp from the last received anchor response to calculate the delay needed to transmit final message.
  tagSendDelayTime = (anchorsTimestamps[ANCHORS_TOTAL_COUNT - 1] + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
  dwt_setdelayedtrxtime(tagSendDelayTime);

  return tagSendDelayTime;
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
static void writeFinalMsg(uint32 tagSendDelayTime) {
  /* Retrieve the timestamp from when the initial message transmits. */
  tagTxTimestamp1 = getTxTimestampU64();
  printf("Tag TX 1 = %u\r\n", (uint32)tagTxTimestamp1);
  
  tagSendDelayTime = setFinalTxDelay();

  /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
  tagTxTimestamp2 = (((uint64)(tagSendDelayTime & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;
  printf("Tag TX 2 = %u\r\n", (uint32)tagTxTimestamp2);

  /* Write all the timestamps in the final message. See NOTE 11 below. */
  finalMsgSetTs(&tagFinalMsg[FINAL_MSG_TX_1_IDX], tagTxTimestamp1);
  finalMsgSetTs(&tagFinalMsg[FINAL_MSG_TX_2_IDX], tagTxTimestamp2);
  finalMsgSetRxTs(&tagFinalMsg[FINAL_MSG_RX_1_IDX]);

  /* Increment frame sequence number after transmission of the final message (modulo 256). */
  exchangeSeqCount++;
  tagFinalMsg[EX_SEQ_COUNT_IDX] = exchangeSeqCount;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn sendInitiationMsg()
 *
 * @brief Transmit the initiation message to begin exchange.
 *
 * @param  none
 *
 * @return the status code of transmission
 */
static int sendInitiationMsg(void) {
  int ret;

  /* Write frame data to DW1000 and prepare transmission. See NOTE 8 below. */
  tagFirstMsg[EX_SEQ_COUNT_IDX] = exchangeSeqCount;
  tagFirstMsg[ANCH_COUNT_IDX] = (uint8) ANCHORS_TOTAL_COUNT;
  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
  dwt_writetxdata(sizeof(tagFirstMsg), tagFirstMsg, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(sizeof(tagFirstMsg), 0, 1); /* Zero offset in TX buffer, ranging. */

  /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent. */
  ret = dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
  if (ret == DWT_SUCCESS) {
    /* Ensure transmission occurs. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};
    txCount++;
    // printf("Transmission # : %d\r\n", txCount);
    return INITIATION_SEND_SUCCESS;
  } else {
    // printf("Transmission # : %d - FAILURE\r\n", txCount);
    return INITIATION_SEND_FAILURE;
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn sendFinalMsg()
 *
 * @brief Transmit the final message to deliver TX/RX timestamps to respective Anchors.
 *
 * @param  none
 *
 * @return the status code of transmission
 */
static int sendFinalMsg() {
  int txStatus;

  /* Write and send final message. See NOTE 8 below. */
  dwt_writetxdata(sizeof(tagFinalMsg), tagFinalMsg, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(sizeof(tagFinalMsg), 0, 1); /* Zero offset in TX buffer, ranging. */
  txStatus = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);

  /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 12 below. */
  if (txStatus == DWT_SUCCESS) {
    /* Poll DW1000 until TX frame sent event set. See NOTE 9 below. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};
    // printf("Final Frame Sent\r\n");
    /* Clear TXFRS event. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

    // printf("Exchange #%u completed.\r\n\r\n", exchangeSeqCount);
    
    return FINAL_SEND_SUCCESS;
  } else {
    return FINAL_SEND_FAILURE;
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn receiveAnchorResponse()
 *
 * @brief Poll for reception of responses from Anchors after transmission of initiation message.
 *
 * @param  none
 *
 * @return the status code of reception
 */
static int receiveAnchorResponse(void) {
  /* PROBLEM: Sometimes it does not timeout. */
  /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
  // printf("Attempting to receive response from all anchors...\r\n");
  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  if (statusReg & SYS_STATUS_RXRFTO) {
    /* Clear RX timeout events before next exchange. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);

    dwt_rxreset();

    printf("=== ERROR ===\r\nRX timeout occured from listening responses. Abandoning current exchange.\r\n");
    printf("Missing Anchors:");
    int i;
    for (i = 0; i < ANCHORS_TOTAL_COUNT; i++) {
      if (anchorsTimestamps[i] == 0) {
        printf("% d |", i + 1);
      }
    }
    printf("\r\n");
    printf("*************\r\n");
    return ANCHOR_RECEIVE_TIMEOUT;
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

    /* Check that the frame is the expected response from the companion "DS TWR responder" example.
    * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;
    if (memcmp(rxBuffer, anchorMsg, ALL_MSG_COMMON_LEN) == 0) {
      rxCount++;
      // printf("Reception # : %d\r\n",rxCount);
      uint8 anchorID;

      /* Retrieve poll transmission and response reception timestamps. See NOTE 5 below. */
      tagRxTimestamp1 = getRxTimestampU64();
      printf("Tag RX 1 = %u\r\n", (uint32)tagRxTimestamp1);

      /* Retrieve the anchor number embedded in the response message. */
      anchorID = rxBuffer[ANCHOR_ID_IDX];

      /* Temporarily store the timestamps specific to the retrieved anchor number. */
      // Safety check
      if (anchorID > ANCHORS_TOTAL_COUNT) {
        printf("=== Error === Anchor number out of bounds. Anchor ID: %u\r\n", anchorID);
      } else {
        anchorsTimestamps[anchorID - 1] = tagRxTimestamp1;
        // printf("Received Anchor ID: %u\r\n", anchorID);
        anchorsCount++;
      }

      /* We need to ensure we enable RX only when we expect another response from the anchors.
        * This is crucial since enabling RX without receiving will cause any transmissions (final
        * message transmission later) to be delayed for unusually long. */
      if (anchorsCount < ANCHORS_TOTAL_COUNT) {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
      }

      return ANCHOR_RECEIVE_SUCCESS;
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      dwt_rxreset();

      printf("=== Error === Anchor Response Frame Incorrect\r\n");
      return ANCHOR_RECEIVE_FAILURE;
    }
  }

  if (statusReg & SYS_STATUS_ALL_RX_ERR) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
    dwt_rxreset();
    printf("=== Error === Frame Received Error (Anchor Response)\r\n");
    return ANCHOR_RECEIVE_FAILURE;
  }

  /* Default return value. */
  return ANCHOR_RECEIVE_FAILURE;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn receiveDistanceMsgs()
 *
 * @brief Poll for reception of distance values from every Anchors for current exchange.
 *
 * @param  none
 *
 * @return the status code of reception
 */
static int receiveDistanceMsgs() {
  /* Poll for reception of distance from each anchor. */
  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  if (statusReg & SYS_STATUS_RXRFTO) {
    /* Clear RX timeout events before next exchange. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);

    dwt_rxreset();

    printf("=== ERROR ===\r\nRX timeout occured from listening distances. Abandoning current exchange.\r\n");
    printf("Missing Anchors:");
    int i;
    for (i = 0; i < ANCHORS_TOTAL_COUNT; i++) {
      if (anchorsDistances[i] == 0) {
        printf("% d |", i + 1);
      }
    }
    printf("\r\n");
    printf("*************\r\n");
    return DISTANCE_RECEIVE_TIMEOUT;
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

    /* Check that the frame is the expected response from the companion "DS TWR responder" example.
    * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;
    if (memcmp(rxBuffer, anchorDistMsg, ALL_MSG_COMMON_LEN) == 0) {
      rxCount++;
      // printf("Distance Reception # : %d\r\n",rxCount);
      double anchorDistance;
      uint8 anchorID;

      /* Retrieve the anchor number embedded in the response message. */
      anchorID = rxBuffer[ANCHOR_ID_IDX];

      /* Retrieve the distance. */
      memcpy(&anchorDistance, &rxBuffer[ANCHOR_DIST_IDX], sizeof(double));

      /* Temporarily store the timestamps specific to the retrieved anchor number. */
      // Safety check
      if (anchorID > ANCHORS_TOTAL_COUNT) {
        printf("=== Error === Anchor number out of bounds. Anchor ID: %u\r\n", anchorID);
      } else {
        anchorsDistances[anchorID - 1] = anchorDistance;
        // printf("Received Anchor ID: %u\r\n", anchorID);
        anchorsCount++;
      }

      /* We need to ensure we enable RX only when we expect another response from the anchors.
        * This is crucial since enabling RX without receiving will cause any transmissions (final
        * message transmission later) to be delayed for unusually long. */
      if (anchorsCount < ANCHORS_TOTAL_COUNT) {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
      }

      return DISTANCE_RECEIVE_SUCCESS;
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      dwt_rxreset();

      printf("=== Error === Anchor Distance Frame Incorrect\r\n");
      return DISTANCE_RECEIVE_FAILURE;;
    }
  }

  if (statusReg & SYS_STATUS_ALL_RX_ERR) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
    dwt_rxreset();
    printf("=== Error === Frame Received Error (Anchor Distance)\r\n");
    return DISTANCE_RECEIVE_FAILURE;
  }

  /* Default return value. */
  return DISTANCE_RECEIVE_FAILURE;
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The sum of the values is the TX to RX antenna delay, experimentally determined by a calibration process. Here we use a hard coded typical value
 *    but, in a real application, each device should have its own antenna delay properly calibrated to get the best possible precision when performing
 *    range measurements.
 * 2. The messages here are similar to those used in the DecaRanging ARM application (shipped with EVK1000 kit). They comply with the IEEE
 *    802.15.4 standard MAC data frame encoding and they are following the ISO/IEC:24730-62:2013 standard. The messages used are:
 *     - a poll message sent by the initiator to trigger the ranging exchange.
 *     - a response message sent by the responder allowing the initiator to go on with the process
 *     - a final message sent by the initiator to complete the exchange and provide all information needed by the responder to compute the
 *       time-of-flight (distance) estimate.
 *    The first 10 bytes of those frame are common and are composed of the following fields:
 *     - byte 0/1: frame control (0x8841 to indicate a data frame using 16-bit addressing).
 *     - byte 2: sequence number, incremented for each new frame.
 *     - byte 3/4: PAN ID (0xDECA).
 *     - byte 5/6: destination address, see NOTE 3 below.
 *     - byte 7/8: source address, see NOTE 3 below.
 *     - byte 9: function code (specific values to indicate which message it is in the ranging process).
 *    The remaining bytes are specific to each message as follows:
 *    Poll message:
 *     - no more data
 *    Response message:
 *     - byte 10: activity code (0x02 to tell the initiator to go on with the ranging exchange).
 *     - byte 11/12: activity parameter, not used here for activity code 0x02.
 *    Final message:
 *     - byte 10 -> 13: poll message transmission timestamp.
 *     - byte 14 -> 17: response message reception timestamp.
 *     - byte 18 -> 21: final message transmission timestamp.
 *    All messages end with a 2-byte checksum automatically set by DW1000.
 * 3. Source and destination addresses are hard coded constants in this example to keep it simple but for a real product every device should have a
 *    unique ID. Here, 16-bit addressing is used to keep the messages as short as possible but, in an actual application, this should be done only
 *    after an exchange of specific messages used to define those short addresses for each device participating to the ranging exchange.
 * 4. Delays between frames have been chosen here to ensure proper synchronisation of transmission and reception of the frames between the initiator
 *    and the responder and to ensure a correct accuracy of the computed distance. The user is referred to DecaRanging ARM Source Code Guide for more
 *    details about the timings involved in the ranging process.
 * 5. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete response frame sent by the responder at the
 *    110k data rate used (around 3 ms).
 * 6. The preamble timeout allows the receiver to stop listening in situations where preamble is not starting (which might be because the responder is
 *    out of range or did not receive the message to respond to). This saves the power waste of listening for a message that is not coming. We
 *    recommend a minimum preamble timeout of 5 PACs for short range applications and a larger value (e.g. in the range of 50% to 80% of the preamble
 *    length) for more challenging longer range, NLOS or noisy environments.
 * 7. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 8. dwt_writetxdata() takes the full size of the message as a parameter but only copies (size - 2) bytes as the check-sum at the end of the frame is
 *    automatically appended by the DW1000. This means that our variable could be two bytes shorter without losing any data (but the sizeof would not
 *    work anymore then as we would still have to indicate the full length of the frame to dwt_writetxdata()).
 * 9. We use polled mode of operation here to keep the example as simple as possible but all status events can be used to generate interrupts. Please
 *    refer to DW1000 User Manual for more details on "interrupts". It is also to be noted that STATUS register is 5 bytes long but, as the event we
 *    use are all in the first bytes of the register, we can use the simple dwt_read32bitreg() API call to access it instead of reading the whole 5
 *    bytes.
 * 10. As we want to send final TX timestamp in the final message, we have to compute it in advance instead of relying on the reading of DW1000
 *     register. Timestamps and delayed transmission time are both expressed in device time units so we just have to add the desired response delay to
 *     response RX timestamp to get final transmission time. The delayed transmission time resolution is 512 device time units which means that the
 *     lower 9 bits of the obtained value must be zeroed. This also allows to encode the 40-bit value in a 32-bit words by shifting the all-zero lower
 *     8 bits.
 * 11. In this operation, the high order byte of each 40-bit timestamps is discarded. This is acceptable as those time-stamps are not separated by
 *     more than 2**32 device time units (which is around 67 ms) which means that the calculation of the round-trip delays (needed in the
 *     time-of-flight computation) can be handled by a 32-bit subtraction.
 * 12. When running this example on the EVB1000 platform with the RESP_RX_TO_FINAL_TX_DLY response delay provided, the dwt_starttx() is always
 *     successful. However, in cases where the delay is too short (or something else interrupts the code flow), then the dwt_starttx() might be issued
 *     too late for the configured start time. The code below provides an example of how to handle this condition: In this case it abandons the
 *     ranging exchange to try another one after 1 second. If this error handling code was not here, a late dwt_starttx() would result in the code
 *     flow getting stuck waiting for a TX frame sent event that will never come. The companion "responder" example (ex_05b) should timeout from
 *     awaiting the "final" and proceed to have its receiver on ready to poll of the following exchange.
 ****************************************************************************************************************************************************/
