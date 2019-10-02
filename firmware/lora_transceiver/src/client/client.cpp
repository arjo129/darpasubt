#include <RH_RF95.h>
#include "common.h"
#include "client.h"
#include "transceiver.h"
#include "writer.h"

/**
 * @brief Construct a new Client:: Client object
 * 
 * @param address unique identifier for this Client.
 * @param receivePin pin on Arduino to receive UART from module.
 * @param transmitPin pin on Arduino to transmit UART to module.
 * @param frequency operation frequency of the module.
 */
Client::Client (uint8_t address, uint8_t receivePin, uint8_t transmitPin, float frequency)
    :
    softSerial(receivePin, transmitPin),
    client(softSerial),
    address(address),
    frequency(frequency)
{
}

/**
 * @brief Initialises the client instance.
 * 
 * @return true if successful.
 * @return false if failed.
 */
bool Client::init(void)
{
  Serial.println("Initialising RF98 CLIENT");
  if (!client.init())
  {
    return false;
  }

  Serial.print("Setting Frequency: ");
  Serial.println(frequency);
  client.setFrequency(frequency);
  client.setThisAddress(address);
  Serial.println("Initialisation success.");

  return true;
}

/**
 * @brief Gets the address of the client.
 * 
 * @return uint8_t the address of the client.
 */
uint8_t Client::getAddress(void)
{
  return address;
}

/**
 * @brief Loop function that is to be ran in the main loop function.
 * 
 */
void Client::loop(void)
{
  uint8_t toSend[MAX_DATA_LEN];
  uint8_t origin;

  writeMsg(&client, toSend, MAX_DATA_LEN, ADDRESS, 0, "Requesting coordinates"); // Send to node with address '0'.
  sendData(&client, toSend, MAX_DATA_LEN);
  
  // Wait for acknowledgement from server
  uint8_t buf[MAX_DATA_LEN];
  bool ack;

  ack = waitData(&client, buf, MAX_DATA_LEN, CL_WAIT_TIME, &origin);
  if (ack)
  {
    Serial.print("Acknowledge from ");
    Serial.print(origin);
    Serial.print(": ");
    Serial.println((char*)buf);
  }
  
  delay(1000);
}