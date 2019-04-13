#include <RH_RF95.h>
#include <common.h>

/**
 * @brief Writes the message to be transmitted.
 * 
 * @param radioDriver the instance of the radio driver in operation.
 * @param msg the buffer to write the data into.
 * @param sourceAddr address of source node.
 * @param destAddr address of destination node.
 * @param data the data to be written.
 */
void writeMsg(RH_RF95 *radioDriver, uint8_t msg[MAX_DATA_LEN], uint8_t sourceAddr, uint8_t destAddr, String data)
{
  // Set the header addresses
  radioDriver->setHeaderFrom(sourceAddr);
  radioDriver->setHeaderTo(destAddr);

  // TODO: Write unique data into the message array.
  if (data.length() > MAX_DATA_LEN)
  {
    return; // Fail silently.
  }
  uint8_t *eg;
  eg = (uint8_t*)data.c_str();
  memcpy(msg, eg, MAX_DATA_LEN);
}