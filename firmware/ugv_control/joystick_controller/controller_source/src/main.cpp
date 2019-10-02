#include <Arduino.h>
#include <DataPackage.h>
#include <SoftwareSerial.h>
#include <RH_RF95.h>

// Joystick related.
#define JS_1_X A0
#define JS_1_Y A1
#define JS_1_SW 4
#define JS_2_X A2
#define JS_2_Y A3
#define JS_2_SW 7

// Transmitter radio driver related.
#define RF_RX_PIN 5
#define RF_TX_PIN 6

// Transmitter radio driver related.
SoftwareSerial ss(RF_RX_PIN, RF_TX_PIN);
RH_RF95 transmitter(ss);
uint8_t buf[DataPackage::DATA_LEN];
DataPackage::Packet data;

void setup() {
  Serial.begin(9600);

  // Setup all the joystick pins.
  pinMode(JS_1_X, INPUT);
  pinMode(JS_1_Y, INPUT);
  pinMode(JS_1_SW, INPUT_PULLUP);
  pinMode(JS_2_X, INPUT);
  pinMode(JS_2_Y, INPUT);
  pinMode(JS_2_SW, INPUT_PULLUP);

  // Initialise transmitter.
  while (!transmitter.init())
  {
    Serial.println("Transmitter failed, trying again in 2 seconds...");
    delay(2000);
  }
  transmitter.setFrequency(434.0);
}

void loop() {
  // Read all joystick inputs as 2 bytes (16 bits) and then serialise them into a buffer array.
  data.js1X = (uint16_t)analogRead(JS_1_X);
  data.js1Y = (uint16_t)analogRead(JS_1_Y);
  data.js1SW = (uint16_t)digitalRead(JS_1_SW);
  data.js2X = (uint16_t)analogRead(JS_2_X);
  data.js2Y = (uint16_t)analogRead(JS_2_Y);
  data.js2SW = (uint16_t)digitalRead(JS_2_SW);
  DataPackage::serialiseData(data, buf);

  // Now we send out the data.
  transmitter.send(buf, DataPackage::DATA_LEN);
  transmitter.waitPacketSent();
}
