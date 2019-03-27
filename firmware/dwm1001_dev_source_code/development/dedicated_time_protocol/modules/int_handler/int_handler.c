#include "nrf_drv_gpiote.h"
#include "nrf_gpiote.h"
#include "dw1001_dev.h"
#include "app_error.h"
#include "deca_device_api.h"
#include "message_transceiver.h"
#include "main.h"

/* Local functions prototypes */
void vInterruptHandler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
extern int counter; // debugging purpose
extern bool isInitiating;
extern bool tx1Sending;
extern bool tx2Sending;
extern bool waitingTx1;
extern bool waitingTx2;

typedef unsigned long long uint64;

/* Public functions */
/**
 * @brief Configure an IO pin as a positive edge triggered interrupt source.
 * 
 */
void vInterruptInit (void)
{
  ret_code_t err_code;

  if (nrf_drv_gpiote_is_init())
    printf("nrf_drv_gpiote_init already installed\n");
  else
    nrf_drv_gpiote_init();

  // input pin, +ve edge interrupt, no pull-up
  nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
  in_config.pull = NRF_GPIO_PIN_NOPULL;

  // Link this pin interrupt source to its interrupt handler
  err_code = nrf_drv_gpiote_in_init(DW1000_IRQ, &in_config, vInterruptHandler);
  APP_ERROR_CHECK(err_code);

  nrf_drv_gpiote_in_event_enable(DW1000_IRQ, true);
}

/**
 * @brief Callback to process RX good frame events
 *
 * @param  cb_data  callback data
 *
 * @return  none
 */
void rx_ok_cb(const dwt_cb_data_t *cb_data)
{
  RxStatus rxStatus;
  uint8 buffer[MSG_LEN] = {0};
  msg_template msg;

  // Eg: Read received frame data and print
  rxStatus = rxMsg(buffer);
  if (rxStatus == RX_SUCCESS) {
    counter++; // debugging purpose
    // Cast the frame to the message structure first
    convertToStruct(buffer, &msg);
    // printf("\tRX: %d, %d\r\n", msg.id, (msg.isFirst == 1 ? 1 : 2));
    rxHandler(&msg);
    resetRxTimeout(msg.id);
  }
  else
  {
    // printf("\tRX fail\r\n");
  }
  

  // Make sure to enable receiver again
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

/**
* @brief Callback to process RX timeout events
*
* @param  cb_data  callback data
*
* @return  none
*/
void rx_to_cb(const dwt_cb_data_t *cb_data)
{
  dwt_rxreset();

  if (waitingTx1)
  {
    waitingTx1 = false;
    configTx1();
  }
  else if (waitingTx2)
  {
    waitingTx2 = false;
    configTx2();
  }
  else
  {
    // Will not reach here.
  }
}

/**
 * @brief Callback to process RX error events
 *
 * @param  cb_data  callback data
 *
 * @return  none
 */
void rx_err_cb(const dwt_cb_data_t *cb_data)
{
  dwt_rxreset();
  dwt_rxenable(DWT_START_RX_IMMEDIATE);
}


static uint64 get_tx_timestamp_u64(void)
{
  uint8 ts_tab[5];
  uint64 ts = 0;
  int i;
  dwt_readtxtimestamp(ts_tab);
  for (i = 4; i >= 0; i--)
  {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}


/**
 * @brief Callback to process TX confirmation events
 *
 * @param  cb_data  callback data
 *
 * @return  none
 */
void tx_conf_cb(const dwt_cb_data_t *cb_data)
{
  /* This callback has been defined so that a breakpoint can be put here to check it is correctly called but there is actually nothing specific to
  * do on transmission confirmation in this example. Typically, we could activate reception for the response here but this is automatically handled
  * by DW1000 using DWT_RESPONSE_EXPECTED parameter when calling dwt_starttx().
  * An actual application that would not need this callback could simply not define it and set the corresponding field to NULL when calling
  * dwt_setcallbacks(). The ISR will not call it which will allow to save some interrupt processing time. */

  if (tx1Sending)
  {
    // printf("TX 1\r\n");
    
    uint64 ts64 = get_tx_timestamp_u64();
    uint32 ts = (uint32)(ts64 >> 8);
    updateTx1Ts(ts);
    setRxTimeout2();

    tx1Sending = false;
    waitingTx2 = true;
  }
  else if (tx2Sending)
  {
    // printf("TX 2\r\n");
    uint64 ts64 = get_tx_timestamp_u64();
    uint32 ts = (uint32)(ts64 >> 8);

    // Make sure device is in IDLE before changing RX timeout.
    dwt_forcetrxoff();
    dwt_setrxtimeout(0);
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
    
    tx2Sending = false;
  }
  else
  {
    // Will not reach here.
  }
  
}

/* Local functions */
/**
 * @brief Interrupt handler calls the DW1000 ISR API. Call back corresponding to each event defined in ss_init_main
 * 
 * @param pin 
 * @param action 
 */
void vInterruptHandler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
  dwt_isr(); // DW1000 interrupt service routine 
}
