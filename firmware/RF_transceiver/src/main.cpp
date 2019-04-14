#include <SoftwareSerial.h>
#include <RH_RF95.h>
#include "common.h"
#include "client.h"
#include "server.h"

Server *server;
Client *client;

void setup() 
{
  Serial.begin(BAUD_RATE);
  
  if (OP_MODE == 1)
  {
    client = new Client(ADDRESS, PIN_TX, PIN_RX, CLIENT_FREQ);
    client->init();
  }
  else
  {
    server = new Server(ADDRESS, PIN_TX, PIN_RX, SERVER_FREQ);
    server->init();

  }
}

void loop()
{
   if (OP_MODE == 1)
   {
     client->loop();
   } 
   else
   {
    server->receive();
   }
}
