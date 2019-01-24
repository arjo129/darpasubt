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
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#define APP_NAME "DS TWR INITIATOR"

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 100

/* Frames used in the ranging process. See NOTE 2 below. */
static uint8 initiatorFirstMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0};
static uint8 responderMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 initiatorFinalMsg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Length of the common part of the message (up to and including the function code, see NOTE 1 below). */
#define ALL_MSG_COMMON_LEN 10

/* Indexes to access some of the fields in the frames defined above. */
#define ALL_MSG_SN_IDX 2
#define FINAL_MSG_TX_1_IDX 10
#define FINAL_MSG_RX_1_IDX 14
#define FINAL_MSG_TX_2_IDX 18
#define FINAL_MSG_TS_LEN 4

/* Exchange sequence number, incremented after each transmission of the final message. */
static uint8 exchangeSeqNum = 0;

/* Buffer to store received response message.
* Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 20
static uint8 rxBuffer[RX_BUF_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
* 1 uus = 512 / 499.2 �s and 1 �s = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 150
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3100
/* Receive response timeout. See NOTE 5 below. */
#define RESP_RX_TIMEOUT_UUS 2700
/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
#define PRE_TIMEOUT 8

/* Time-stamps of frames transmission/reception, expressed in device time units.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef unsigned long long uint64;
static uint64 initTxTimestamp1;
static uint64 initRxTimestamp1;
static uint64 initTxTimestamp2;

/* Declaration of static functions. */
static uint64 getTxTimestampU64(void);
static uint64 getRxTimestampU64(void);
static void finalMsgSetTs(uint8 *tsField, uint64 ts);

/*Transactions Counters */
static volatile int tx_count = 0 ; // Successful transmit counter
static volatile int rx_count = 0 ; // Successful receive counter 


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

  /* Write frame data to DW1000 and prepare transmission. See NOTE 8 below. */
  initiatorFirstMsg[ALL_MSG_SN_IDX] = exchangeSeqNum;
  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
  dwt_writetxdata(sizeof(initiatorFirstMsg), initiatorFirstMsg, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(sizeof(initiatorFirstMsg), 0, 1); /* Zero offset in TX buffer, ranging. */

  /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent and the delay
  * set by dwt_setrxaftertxdelay() has elapsed. */
  dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
  tx_count++;
  printf("Transmission # : %d\r\n",tx_count);

  printf("Attempting to receive second frame...\r\n");
  /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
  while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR))) {};

  if (status_reg & SYS_STATUS_RXFCG) {		
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
    rxBuffer[ALL_MSG_SN_IDX] = 0;
    if (memcmp(rxBuffer, responderMsg, ALL_MSG_COMMON_LEN) == 0) {	
      rx_count++;
      printf("Reception # : %d\r\n",rx_count);
      uint32 initSendDelayTime;
      int ret;

      /* Retrieve poll transmission and response reception timestamps. See NOTE 5 below. */
      initTxTimestamp1 = getTxTimestampU64();
      initRxTimestamp1 = getRxTimestampU64();

      /* Compute final message transmission time. See NOTE 10 below. */
      initSendDelayTime = (initRxTimestamp1 + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
      dwt_setdelayedtrxtime(initSendDelayTime);

      /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
      initTxTimestamp2 = (((uint64)(initSendDelayTime & 0xFFFFFFFEUL)) << 8) + 16436;

      /* Write all the timestamps in the final message. See NOTE 11 below. */
      finalMsgSetTs(&initiatorFinalMsg[FINAL_MSG_TX_1_IDX], initTxTimestamp1);
      finalMsgSetTs(&initiatorFinalMsg[FINAL_MSG_RX_1_IDX], initRxTimestamp1);
      finalMsgSetTs(&initiatorFinalMsg[FINAL_MSG_TX_2_IDX], initTxTimestamp2);

      /* Increment frame sequence number after transmission of the final message (modulo 256). */
      exchangeSeqNum++;

      /* Write and send final message. See NOTE 8 below. */
      initiatorFinalMsg[ALL_MSG_SN_IDX] = exchangeSeqNum;
      dwt_writetxdata(sizeof(initiatorFinalMsg), initiatorFinalMsg, 0); /* Zero offset in TX buffer. */
      dwt_writetxfctrl(sizeof(initiatorFinalMsg), 0, 1); /* Zero offset in TX buffer, ranging. */
      ret = dwt_starttx(DWT_START_TX_DELAYED);

      /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 12 below. */
      if (ret == DWT_SUCCESS) {
        /* Poll DW1000 until TX frame sent event set. See NOTE 9 below. */
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};

        /* Clear TXFRS event. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

      }
    }
  } else {
    /* Clear RX error/timeout events in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

    /* Reset RX to properly reinitialise LDE operation. */
    dwt_rxreset();
  }

  /* Execute a delay between ranging exchanges. */
  //     deca_sleep(RNG_DELAY_MS);

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

/**@brief SS TWR Initiator task entry function.
*
* @param[in] pvParameter   Pointer that will be used as the parameter for the task.
*/
void ds_initiator_task_function (void * pvParameter) {
  UNUSED_PARAMETER(pvParameter);

  //dwt_setrxtimeout(RESP_RX_TIMEOUT_UUS);

  dwt_setleds(DWT_LEDS_ENABLE);

  while (true) {
    dsInitRun();
    /* Delay a task for a given number of ticks */
    vTaskDelay(RNG_DELAY_MS);
    /* Tasks must be implemented to never return... */
  }
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
