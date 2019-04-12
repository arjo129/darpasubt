#include <SoftwareSerial.h>
#include <RH_RF95.h>
#include "common.h"
#include "client.h"
#include "server.h"

// Singleton instance of the radio driver
SoftwareSerial ss(PIN_TX, PIN_RX);
RH_RF95 radioDriver(ss);

void setup() 
{
  Serial.begin(BAUD_RATE);
  
  if (OP_MODE == 1)
  {
    clientSetup(&radioDriver);
  }
  else
  {
    serverSetup(&radioDriver);
  }
}

void loop()
{
   if (OP_MODE == 1)
   {
     clientLoop(&radioDriver);
   } 
   else
   {
    serverReceive(&radioDriver);
   }
}
