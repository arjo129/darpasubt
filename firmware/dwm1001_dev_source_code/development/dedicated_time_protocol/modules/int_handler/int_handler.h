#include "deca_device_api.h"

/* Public function prototypes */
void vInterruptInit (void);
void rx_ok_cb(const dwt_cb_data_t *cb_data);
void rx_to_cb(const dwt_cb_data_t *cb_data);
void rx_err_cb(const dwt_cb_data_t *cb_data);
void tx_conf_cb(const dwt_cb_data_t *cb_data);