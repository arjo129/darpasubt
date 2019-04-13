#include <RH_RF95.h>
#include "common.h"

void writeMsg(RH_RF95 *radioDriver, uint8_t msg[MAX_DATA_LEN], uint8_t sourceAddr, uint8_t destAddr, String data);