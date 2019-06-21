#include "deca_device_api.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpiote.h"
#include "dw1001_dev.h"
#include "app_error.h"
#include "deca_device_api.h"
#include "timestamper.h"

#ifndef MESSAGE_TRANSCEIVER_H
#define MESSAGE_TRANSCEIVER_H
#include "message_transceiver.h"
#endif

#ifndef MESSAGE_TEMPLATE_H
#define MESSAGE_TEMPLATE_H
#include "message_template.h"
#endif

/* Public function prototypes */
void vInterruptInit (void);
void rx_ok_cb(const dwt_cb_data_t *cb_data);
void rx_to_cb(const dwt_cb_data_t *cb_data);
void rx_err_cb(const dwt_cb_data_t *cb_data);
void tx_conf_cb(const dwt_cb_data_t *cb_data);