#include <RH_RF95.h>
#include "common.h"
#include "server.h"
#include "transceiver.h"
#include "writer.h"

/**
 * @brief Construct a new Server:: Server object
 * 
 * @param address unique identifier for this Server.
 * @param receivePin pin on Arduino to receive UART from module.
 * @param transmitPin pin on Arduino to transmit UART to module.
 * @param frequency operation frequency of the module.
 */
Server::Server(uint8_t address, uint8_t receivePin, uint8_t transmitPin, float frequency)
{
  this->address = address;
  this->frequency = frequency;

  // Initialise the instance of the radio driver.
  softSerial = new SoftwareSerial(receivePin, transmitPin);
  server = new RH_RF95(*softSerial);

  pinMode(LED_PIN, OUTPUT);
}

/**
 * @brief Initialises the server instance.
 * 
 * @return true if successful.
 * @return false if failed.
 */
bool Server::init(void)
{
  Serial.println("Initialising RF98 SERVER");
  if (!server->init())
  {
    return false;
  }

  Serial.print("Setting Frequency: ");
  Serial.println(frequency);
  server->setFrequency(frequency);
  server->setThisAddress(address);
  Serial.println("Initialisation success.");

  return true;
}

/**
 * @brief Gets the address of the server.
 * 
 * @return uint8_t the address of the server.
 */
uint8_t Server::getAddress(void)
{
  return address;
}

/**
 * @brief Function to receive data once it is available on UART.
 * 
 * @param server the instance of the radio driver in operation.
 */
void Server::receive(void)
{
  uint8_t buf[MAX_DATA_LEN];
  uint8_t origin;
  bool recv;
  
  // Check if there are data in the reception buffer.
  if (!server->available())
  {
    return;
  }

  recv = waitData(server, buf, MAX_DATA_LEN, 0, &origin); // set timeout to continuously find messages
  if (recv)
  {
    digitalWrite(LED_PIN, HIGH);
    
    Serial.print("Request from ");
    Serial.print(origin);
    Serial.print(": ");
    Serial.println((char*)buf);
    
    // Send acknowledgement
    uint8_t toSend[MAX_DATA_LEN];
    writeMsg(server, toSend, MAX_DATA_LEN, ADDRESS, 1, "Message acknowledged"); // Send to node with address '1'.
    sendData(server, toSend, MAX_DATA_LEN);
    
    digitalWrite(LED_PIN, LOW);
  }
}
