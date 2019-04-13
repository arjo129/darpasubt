#include <RH_RF95.h>
#include "common.h"
#include "client.h"
#include "transceiver.h"
#include "writer.h"

/**
 * @brief Setup function to configure as client operation.
 * 
 * @param client the instance of the radio driver in operation.
 */
void clientSetup(RH_RF95 *client)
{
  Serial.println("RF98 CLIENT Setup");

  if (!client->init())
  {
    Serial.println("Client setup failed, please restart.");
    while(1);
  }

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  //client->setTxPower(13, false);

  Serial.println("Setup success.");
  Serial.print("Setting Frequency: ");
  Serial.println(CLIENT_FREQ);
  client->setFrequency(CLIENT_FREQ);
  client->setThisAddress(ADDRESS);
}

/**
 * @brief Loop function that is to be ran in the main loop function.
 * 
 * @param client the instance of the radio driver in operation.
 */
void clientLoop(RH_RF95 *client)
{
  uint8_t toSend[MAX_DATA_LEN];
  uint8_t origin;

  writeMsg(client, toSend, MAX_DATA_LEN, ADDRESS, 0, "Requesting coordinates"); // Send to node with address '0'.
  sendData(client, toSend, MAX_DATA_LEN);
  
  // Wait for acknowledgement from server
  uint8_t buf[MAX_DATA_LEN];
  bool ack;

  ack = waitData(client, buf, MAX_DATA_LEN, CL_WAIT_TIME, &origin);
  if (ack)
  {
    Serial.print("Acknowledge from ");
    Serial.print(origin);
    Serial.print(": ");
    Serial.println((char*)buf);
  }
  
  delay(1000);
}