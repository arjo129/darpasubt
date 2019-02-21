#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"
#include "message_transceiver.h"

/* Local function prototypes. */

/*Interrupt flag*/
extern volatile int tx_int_flag; // Transmit success interrupt flag
extern volatile int rx_int_flag; // Receive success interrupt flag
extern volatile int to_int_flag; // Timeout interrupt flag
extern volatile int er_int_flag; // Error interrupt flag 

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn txMsg()
 *
 * @brief transmit a frame.
 *
 * @param msg - pointer to the message (array) to be transmitted.
 *        mode - transmission mode to be used in transmission.
 *
 * @return the status of transmission.
 */
TxStatus txMsg(uint8 *msg, int msgLen, uint8 mode) {
  int ret;

  /* Write frame data to DW1000 and prepare transmission. */
  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
  dwt_writetxdata(msgLen, msg, 0); /* Zero offset in TX buffer. */
  dwt_writetxfctrl(msgLen, 0, 1);

  /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent. */
  ret = dwt_starttx(mode);
  if (ret == DWT_SUCCESS) {
    /* Ensure transmission occurs. */
    return TX_SUCCESS;
  } else {
    return TX_ERROR;
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn rxMsg()
 *
 * @brief poll for reception of a frame.
 *
 * @param buffer - pointer to buffer that stores the received frame.
 *        msgType - pointer to be used to indicate the type of message in the received frame.
 * 
 * @return the status of reception.
 */
RxStatus rxMsg(uint8 *buffer, MsgType *msgType) {
  *msgType = MSG_TYPE_UNKNOWN; // Default return value

  /* Poll for reception of a frame success/error/timeout or software interrupt. */
  while (!(rx_int_flag || to_int_flag|| er_int_flag)) {};
  if (to_int_flag) {
    to_int_flag = 0;
    return RX_TIMEOUT;
  }

  if (er_int_flag) {
    er_int_flag = 0;
    return RX_ERROR;
  }

  if (rx_int_flag) {
    uint32 frameLen;

    rx_int_flag = 0;

    /* A frame has been received, read it into the local buffer. */
    frameLen = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
    if (frameLen <= RX_BUFFER_LEN) {
      dwt_readrxdata(buffer, frameLen, 0);
    } else {
      return RX_ERROR;
    }

    return RX_SUCCESS;
  }
}

/************************************************
 * LOCAL FUNCTIONS
 * *********************************************/
