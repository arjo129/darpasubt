#include <RH_RF95.h>
#include "common.h"
#include "server.h"
#include "transceiver.h"

/**
 * @brief Setup function to configure as server operation.
 * 
 * @param server the instance of the radio driver in operation.
 */
void serverSetup(RH_RF95 *server)
{
  Serial.println("RF98 SERVER Setup");
    
  pinMode(LED_PIN, OUTPUT); 
  
  if(!server->init())
  {
    Serial.println("Server setup failed, please restart.");
    while(1);
  }

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //server->setTxPower(13, false);
  
  Serial.println("Setup success.");
  Serial.print("Setting Frequency: ");
  Serial.println(SERVER_FREQ);
  server->setFrequency(SERVER_FREQ);
}

/**
 * @brief Function to receive data once it is available on UART.
 * 
 * @param server the instance of the radio driver in operation.
 */
void serverReceive(RH_RF95 *server)
{
  uint8_t buf[MAX_DATA_LEN];
  bool recv;
  
  // Check if there are data in the reception buffer.
  if (!server->available())
  {
    return;
  }

  recv = waitData(server, buf, 0); // set timeout to continuously find messages
  if (recv)
  {
    digitalWrite(LED_PIN, HIGH);
    
    Serial.print("Request: ");
    Serial.println((char*)buf);
    
    // Send acknowledgement
    uint8_t toSend[MAX_DATA_LEN] = "Message acknowledged";
    sendData(server, toSend);
    
    digitalWrite(LED_PIN, LOW);
  }
}