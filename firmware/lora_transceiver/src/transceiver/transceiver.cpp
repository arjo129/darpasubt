#include <RH_RF95.h>
#include "transceiver.h"

/**
 * @brief Transmits given data.
 * 
 * @param radioDriver the instance of the radio driver in operation.
 * @param data pointer to the data to be sent out.
 * @param dataLen length of the data in bytes.
 * @return true if transmission successful.
 * @return false if transmission failed.
 */
bool sendData(RH_RF95 *radioDriver, uint8_t *data, uint8_t dataLen)
{
  bool result;

  Serial.println("Sending message.");
  result = radioDriver->send(data, dataLen);

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
 * @param bufLen length of the buffer array in bytes.
 * @param timeout time duration to wait for data. Set to 0 if immediate receive is required.
 * @param source the origin address of the received message.
 * @return true if message is found and data is received.
 * @return false if no message is found or unable to receive data.
 */
bool waitData(RH_RF95 *radioDriver, uint8_t *buf, uint8_t bufLen, uint16_t timeout, uint8_t *source)
{
  bool result;

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
  result = radioDriver->recv(buf, &bufLen);
  *source = radioDriver->headerFrom();
  if(!result)
  {
    Serial.println("Message found but receive failed.");
  }

  return result;
}