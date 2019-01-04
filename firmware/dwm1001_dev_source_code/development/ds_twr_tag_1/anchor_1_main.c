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
#include "UART.h"

/* Number representing the identification of this Anchor. 
* Minimum of 1 and maximum of 256, ie. a 1-byte value. 
* This number MUST be set in a successive order for all
* anchors in the exchange system, starting from '1'. */
////////////////////*** IMPORTANT ***/////////////////////
///// ENSURE THIS IS CORRECTLY SET BEFORE OPERATION! /////
#define ANCHOR_ID 3

/* Inter-ranging delay period, in milliseconds. See NOTE 1*/
#define RNG_DELAY_SUCCESS_MS 80
#define RNG_DELAY_FAILURE_MS 80

/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10

/* Index to access some of the fields in the frames involved in the process. */
#define EX_SEQ_COUNT_IDX 2
#define ANCH_COUNT_IDX 10
#define FINAL_MSG_TX_1_IDX 10
#define FINAL_MSG_TX_2_IDX 14
#define FINAL_MSG_RX_1_IDX 18
#define ANCHOR_ID_IDX 10
#define ANCHOR_DIST_IDX 11

/* Values to help determine if message transmitted or received. */
#define INITIATION_RECEIVE_INTERRUPTED 3
#define INITIATION_RECEIVE_TIMEOUT 2
#define INITIATION_RECEIVE_SUCCESS 1
#define INITIATION_RECEIVE_FAILURE 0
#define RESPONSE_SEND_SUCCESS 1
#define RESPONSE_SEND_FAILURE 0
#define FINAL_RECEIVE_TIMEOUT 2
#define FINAL_RECEIVE_SUCCESS 1
#define FINAL_RECEIVE_FAILURE 0
#define DISTANCE_SEND_SUCCESS 1
#define DISTANCE_SEND_FAILURE 0
#define EXCHANGE_INTERRUPTED 3
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

/* Delay interval between each successive anchor's reply. */
#define ANCH_INTV_DLY 250000

/* Delay to turn on receiver after transmission of anchor's reply. */
#define ANCH_RX_AFT_TX_DLY 800

/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS  2600

/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 3300

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Use 10 readings to get the average value. */
#define READINGS_COUNT_FOR_AVG 10

/* Frames used in the ranging process. See NOTE 2,3 below. */
static uint8 tagFirstMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0};
static uint8 anchorMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0};
static uint8 tagFinalMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 anchorDistMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Buffer to store received response message.
* Its size is adjusted to longest frame that this example code is supposed to handle. */
static uint8 rxBuffer[RX_BUF_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 statusReg = 0;

/* Timestamps of frames transmission/reception.
* As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef signed long long int64;
typedef unsigned long long uint64;
static uint64 respRxTimestamp1;
static uint64 respTxTimestamp1;
static uint64 respRxTimestamp2;
static uint32 initTxTimestamp1;
static uint32 initRxTimestamp1;
static uint32 initTxTimestamp2;

/* Hold copies of computed time of flight and distance here for reference so that it can be examined at a debug breakpoint. */
static double timeOfFlight;
static double distanceMetre;
static double totalDistances = 0;

/* Total number of anchors in this system, set by the tag. */
static uint8 totalAnchors;

/* Exchange sequence number, incremented after each transmission of the final message. */
static uint8 exchangeSeqCount = 0;

/* A counter for the number of readings computed, so we can compute an average value. */
static int readingCount = 0;

/* Flag to determine if message has been transmitted or received. */
static int receiveInitiation = 0;
static int sendResponse = 0;
static int receiveFinal = 0;
static int sendDistance = 0;

/* Buffer to receive commands from UART RX buffer. */
char commandString[UART_RX_BUF_SIZE] = {0};

/* Flag to notify that there is data in UART RX. */
static bool hasRxData = false;

/* The type of operation for this node. */
static int operationMode;

extern bool hasInterruptEvent;

/* Declaration of static functions. */
static uint64 getTxTimestampU64(void);
static uint64 getRxTimestampU64(void);
static void finalMsgGetTs(const uint8 *tsField, uint32 *ts);
static void writeResponseMsg(void);
static void writeDistanceMsg(void);
static void setResponseDelays(uint64 timestampToDelayFrom);
static int receiveInitiationMsg(void);
static int receiveFinalMsg(void);
static int sendResponseMsg(void);
static int sendDistanceMsg(void);
static void computeDistance(void);

/* Enumerations */
enum OperationMode {
  MODE_TAG = 1,
  MODE_ANCHOR = 2,
  MODE_GATEWAY = 4
};

/*! ------------------------------------------------------------------------------------------------------------------
* @fn main()
*
* @brief Application entry point.
*
* @param  none
*
* @return none
*/

int dsRespRun(void) {
  /* Notifies starting of reception. */
  printf("Anchor ID #%d receiving...\r\n", ANCHOR_ID);

  /* Clear reception timeout to start next ranging process. */
  dwt_setrxtimeout(0);

  /* Activate reception immediately. */
  dwt_rxenable(DWT_START_RX_IMMEDIATE);

  receiveInitiation = receiveInitiationMsg();
  if (receiveInitiation == INITIATION_RECEIVE_FAILURE
      || receiveInitiation == INITIATION_RECEIVE_TIMEOUT) {
    dwt_forcetrxoff(); // Ensure transceiver is off for future usage
    return EXCHANGE_FAILURE;
  } else if (receiveInitiation == INITIATION_RECEIVE_INTERRUPTED) {
    dwt_forcetrxoff(); // Ensure transceiver is off for future usage
    return EXCHANGE_INTERRUPTED;
  }

  writeResponseMsg();
  setResponseDelays(respRxTimestamp1);
  sendResponse = sendResponseMsg();
  if (sendResponse == RESPONSE_SEND_FAILURE) {
    printf("failed to send\r\n");
    return EXCHANGE_FAILURE;
  }

  /* 65ms RX timeout. If a final message from Tag was never received, we just abandon current exchange. */
  dwt_setrxtimeout(65000);
  printf("Timeout value: %u\r\n", dwt_read32bitreg(RX_FWTO_ID));
  int pollForFinal = 1;
  while (pollForFinal) {
    receiveFinal = receiveFinalMsg();
    switch(receiveFinal) {
      case FINAL_RECEIVE_SUCCESS:
        pollForFinal = 0;
        break;
      case FINAL_RECEIVE_TIMEOUT:
        return EXCHANGE_TIMEOUT;
      case FINAL_RECEIVE_FAILURE:
        return EXCHANGE_FAILURE;
      default:
        return EXCHANGE_FAILURE;
    }
  }

  computeDistance();
  printf("Completed Exchange #%u --- Distance: %f m\r\n", exchangeSeqCount, distanceMetre);
  
  writeDistanceMsg();
  setResponseDelays(respRxTimestamp2);
  sendDistance = sendDistanceMsg();
  if (sendDistance == DISTANCE_SEND_SUCCESS) {
    printf("Sent back distance.\r\n\r\n");
  }

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
static void finalMsgGetTs(const uint8 *tsField, uint32 *ts) {
    int i;
    *ts = 0;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++) {
        *ts += tsField[i] << (i * 8);
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn writeResponseMsg()
 *
 * @brief Fill the response message with this Anchor's ID.
 *
 * @param  none
 *
 * @return none
 */
static void writeResponseMsg(void) {
  /* Send ID of this anchor back to tag. */
  anchorMsg[ANCHOR_ID_IDX] = ANCHOR_ID;
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
static void writeDistanceMsg(void) {
  /* Write the ID and distance to transmission message. */
  anchorDistMsg[ANCHOR_ID_IDX] = ANCHOR_ID;
  memcpy(&anchorDistMsg[ANCHOR_DIST_IDX], &distanceMetre, sizeof(double));
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn setResponseDelays()
 *
 * @brief Set the TX delay and RX after TX delay for sending the response message. The calculated values is designed to
 *        stagger the TX moment of each anchor based on their ID.
 *
 * @param  timestampToDelayFrom the RX timestamp used to calculate the delay
 *
 * @return none
 */
static void setResponseDelays(uint64 timestampToDelayFrom) {
  uint32 respSendDelayTime, rxDelay;
  uint64 delay_64, rxDelay64;

  /* Set send time for response. See NOTE 9 below. */
  delay_64 = timestampToDelayFrom + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME);
  delay_64 = delay_64 + ((ANCHOR_ID - 1) * (ANCH_RX_AFT_TX_DLY * UUS_TO_DWT_TIME));
  respSendDelayTime = (delay_64) >> 8;
  dwt_setdelayedtrxtime(respSendDelayTime);

  /* Set the delay to turn on receiver after transmission of respone message. */
  rxDelay = ((totalAnchors - ANCHOR_ID) * ANCH_RX_AFT_TX_DLY) + POLL_RX_TO_RESP_TX_DLY_UUS;
  dwt_setrxaftertxdelay(rxDelay);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn sendResponseMsg()
 *
 * @brief Transmit the response message after reception of initiation message from Tag.
 *
 * @param  none
 *
 * @return the status code of transmission
 */
static int sendResponseMsg(void) {
  int ret;

  /* Write and send the response message. See NOTE 10 below. */
  dwt_writetxdata(sizeof(anchorMsg), anchorMsg, 0);
  dwt_writetxfctrl(sizeof(anchorMsg), 0, 1);
  ret = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);

  /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 11 below. */
  if (ret == DWT_SUCCESS) {
    /* Ensure transmission occurs. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};

    /* Clear TXFRS event. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
    printf("response sent\r\n");
    return RESPONSE_SEND_SUCCESS;
  } else {
    printf("failure\r\n");
    return RESPONSE_SEND_FAILURE;
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn sendDistanceMsg()
 *
 * @brief Transmit the distance message after reception of final message from Tag.
 *
 * @param  none
 *
 * @return the status code of transmission
 */
static int sendDistanceMsg(void) {
  int ret;

  /* Write and send the response message. See NOTE 10 below. */
  dwt_writetxdata(sizeof(anchorDistMsg), anchorDistMsg, 0);
  dwt_writetxfctrl(sizeof(anchorDistMsg), 0, 1);
  ret = dwt_starttx(DWT_START_TX_DELAYED);

  /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 11 below. */
  if (ret == DWT_SUCCESS) {
    /* Ensure transmission occurs. */
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};

    /* Clear TXFRS event. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

    return DISTANCE_SEND_SUCCESS;
  } else {
    return DISTANCE_SEND_FAILURE;
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn receiveInitiationMsg()
 *
 * @brief Poll for reception of initiation from Tag to begin exchange.
 *
 * @param  none
 *
 * @return the status code of reception
 */
static int receiveInitiationMsg(void) {
  /* Poll for reception of a frame or error/timeout. See NOTE 8 below. */
  printf("Attempting to receive initiation message...\r\n", ANCHOR_ID);
  printf("at anchor: interrupt = %d\r\n", hasInterruptEvent);
  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))
      && !hasInterruptEvent) {};

  if (hasInterruptEvent) {
    printf("interrupt detected\r\n");
    hasInterruptEvent = false;
    return INITIATION_RECEIVE_INTERRUPTED;
  }

  if (statusReg & SYS_STATUS_ALL_RX_TO) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);
    printf("=== ERROR ===\r\nRX timeout occurred from final message. Abandoning current exchange.\r\n*************\r\n");
    return INITIATION_RECEIVE_TIMEOUT;
  }

  if (statusReg & SYS_STATUS_RXFCG) {
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
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;
    if (memcmp(rxBuffer, tagFirstMsg, ALL_MSG_COMMON_LEN) == 0) {
      int ret;

      /* Retrieve poll reception timestamp. */
      respRxTimestamp1 = getRxTimestampU64();
      /* Retrieve the total number of anchors in the system. */
      totalAnchors = rxBuffer[ANCH_COUNT_IDX];

      /* Notifies reception of exchange initiation message. */
      printf("Initiation message received.\r\n");

      return INITIATION_RECEIVE_SUCCESS;
    }
  } else {
    /* Clear RX error events in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);

    /* Reset RX to properly reinitialise LDE operation. */
    dwt_rxreset();

    printf("=== Error === Tag Initiation Frame Incorrect\r\n");
    return INITIATION_RECEIVE_FAILURE;
  }

  if (statusReg & SYS_STATUS_ALL_RX_ERR) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
    printf("=== Error === Frame Received Error (Initiation Message)\r\n");
    return RESPONSE_SEND_FAILURE;
  }

  /* Default return. */
  return RESPONSE_SEND_FAILURE;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn receiveFinalMsg()
 *
 * @brief Poll for reception of final message from Tag to retrieve all TX/RX timestamps.
 *
 * @param  none
 *
 * @return the status code of reception
 */
static int receiveFinalMsg(void) {
  /* Poll for reception of expected "final" frame or error/timeout. See NOTE 8 below. */
  printf("Attempting to receive final message...\r\n", ANCHOR_ID);
  while (!((statusReg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  /* Disable RX timeout since we want to look for Tag messages indefinitely after this.
    * Note: calling dwt_setrxtimeout(0) does not set the timeout period register with zero. (see: function description) */
  dwt_forcetrxoff(); // Make sure the device is in IDLE first before setting RX timeout. (see: user manual)
  dwt_setrxtimeout(0);

  if (statusReg & SYS_STATUS_ALL_RX_TO) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO);
    printf("=== ERROR ===r\nRX timeout occurred from final message. Abandoning current exchange.\r\n*************\r\n");
    return FINAL_RECEIVE_TIMEOUT;
  }

  if (statusReg & SYS_STATUS_RXFCG) {
    uint32 frameLen;

    /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

    /* A frame has been received, read it into the local buffer. */
    frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    if (frameLen <= RX_BUF_LEN) {
        dwt_readrxdata(rxBuffer, frameLen, 0);
    }

    /* Get exchange sequence number embedded in the final message. */
    exchangeSeqCount = rxBuffer[EX_SEQ_COUNT_IDX];

    /* Check that the frame is a final message sent by "DS TWR initiator" example.
      * As the sequence number field of the frame is not used in this example, it can be zeroed to ease the validation of the frame. */
    rxBuffer[EX_SEQ_COUNT_IDX] = 0;
    if (memcmp(rxBuffer, tagFinalMsg, ALL_MSG_COMMON_LEN) == 0) {
      int ret;

      /* Retrieve response transmission and final reception timestamps. */
      respTxTimestamp1 = getTxTimestampU64();
      respRxTimestamp2 = getRxTimestampU64();

      /* Get timestamps embedded in the final message. */
      finalMsgGetTs(&rxBuffer[FINAL_MSG_TX_1_IDX], &initTxTimestamp1);
      finalMsgGetTs(&rxBuffer[FINAL_MSG_TX_2_IDX], &initTxTimestamp2);
      finalMsgGetTs(&rxBuffer[FINAL_MSG_RX_1_IDX + ((ANCHOR_ID - 1) * FINAL_MSG_TS_LEN)], &initRxTimestamp1);

      return FINAL_RECEIVE_SUCCESS;
    } else {
      /* Clear RX error/timeout events in the DW1000 status register. */
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

      /* Reset RX to properly reinitialise LDE operation. */
      dwt_rxreset();

      printf("=== Error === Tag Final Frame Incorrect\r\n");
      return FINAL_RECEIVE_FAILURE;
    }
  }

  if (statusReg & SYS_STATUS_ALL_RX_ERR) {
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
    printf("=== Error === Frame Received Error (Final Message)\r\n");
    return FINAL_RECEIVE_FAILURE;
  }

  /* Default return. */
  return FINAL_RECEIVE_FAILURE;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn computeDistance()
 *
 * @brief Calculate the approximate distance between this Anchor and target Tag using the TX/RX timestamp values
 *        received from the final message.
 *
 * @param  none
 *
 * @return none
 */
static void computeDistance(void) {
  uint32 respRxTimestamp1_32, respTxTimestamp1_32, respRxTimestamp2_32;
  double roundTrip1, roundTrip2, replyTrip2, replyTrip1;
  int64 timeOfFlightInUnits;

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
}

void setRxData(uint8_t *data) {
  memcpy(commandString, data, sizeof commandString);
  hasRxData = true;
}

static void decodeRxData(void) {
  if (commandString[0] == 't') {
    operationMode = MODE_TAG;
  } else if (commandString[0] == 'a') {
    operationMode = MODE_ANCHOR;
  } else {
    operationMode = 0;
  }
}

/**@brief DS TWR Anchor task entry function.
*
* @param[in] pvParameter   Pointer that will be used as the parameter for the task.
*/
void ds_responder_task_function (void * pvParameter) {
  /* Default mode */
  operationMode = MODE_ANCHOR;

  UNUSED_PARAMETER(pvParameter);

  dwt_setleds(DWT_LEDS_ENABLE);

  while (true) {
    // Check if there is data in UART RX
    if (hasRxData) {
      decodeRxData();
      memset(commandString, 0, sizeof commandString);
      hasRxData = false;
    }

    if (!(operationMode & (MODE_TAG | MODE_ANCHOR))) {
      vTaskDelay(RNG_DELAY_FAILURE_MS);
      continue;
    }

    ds_resp_run();
    /* Delay a task for a given number of ticks */
    vTaskDelay(RNG_DELAY_SUCCESS_MS);
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
