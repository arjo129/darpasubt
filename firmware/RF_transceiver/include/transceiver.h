#include <RH_RF95.h>

bool sendData(RH_RF95 *radioDriver, uint8_t *data, uint8_t dataLen);
bool waitData(RH_RF95 *radioDriver, uint8_t *buf, uint8_t bufLen, uint16_t timeout, uint8_t *source);
