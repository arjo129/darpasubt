#include <RH_RF95.h>
#include "common.h"

bool sendData(RH_RF95 *radioDriver, uint8_t data[MAX_DATA_LEN]);
bool waitData(RH_RF95 *radioDriver, uint8_t buf[MAX_DATA_LEN], uint16_t timeout, uint8_t *source);
