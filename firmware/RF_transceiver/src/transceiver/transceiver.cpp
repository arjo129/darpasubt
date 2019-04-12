#include <RH_RF95.h>
#include "transceiver.h"

/**
 * @brief Transmits given data.
 * 
 * @param radioDriver the instance of the radio driver in operation.
 * @param data the data to be sent out.
 * @return true if transmission successful.
 * @return false if transmission failed.
 */
bool sendData(RH_RF95 *radioDriver, uint8_t data[MAX_DATA_LEN])
{
  bool result;

  Serial.println("Sending message.");
  result = radioDriver->send(data, MAX_DATA_LEN);

  if (result)
  {
    radioDriver->waitPacketSent();
  }
  else
  {
    Serial.println("Sending failed.");
  }

  return result;
}

/**
 * @brief Waits for a fix duration given by CL_WAIT_TIME until a message is found.
 * 
 * @details The function waits for CL_WAIT_TIME duration.
 *          If no message is found during that time or when the time is up, returns with false.
 *          
 *          The timeout is to be specified in milliseconds.
 * 
 * @param radioDriver the instance of the radio driver in operation.
 * @param buf buffer array to store the received data.
 * @param timeout time duration to wait for data. Set to 0 if immediate receive is required.
 * @return true if message is found and data is received.
 * @return false if no message is found or unable to receive data.
 */
bool waitData(RH_RF95 *radioDriver, uint8_t buf[MAX_DATA_LEN], uint16_t timeout)
{
  bool result;
  uint8_t len = MAX_DATA_LEN;

  // Listen for messages for the timeout duration.
  if (timeout > 0)
  {
    if(!radioDriver->waitAvailableTimeout(timeout)) // Timeout is in milliseconds
    {
      Serial.println("No message received.");
      return false;
    }
  }

  // Retrieve the message.
  result = radioDriver->recv(buf, &len);
  if(!result)
  {
    Serial.println("Message found but receive failed.");
  }

  return result;
}