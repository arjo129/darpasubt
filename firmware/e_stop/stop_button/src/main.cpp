#include <common.h>

SoftwareSerial ss(TX_PIN, RX_PIN);
RH_RF95 node(ss);

void setup() {
  Serial.begin(9600);
  if (!node.init())
  {
    Serial.println("LoRa initialise failed.");
  }
  node.setFrequency(434.0);
  
  pinMode(LOCK_LED, OUTPUT);
  pinMode(UNLOCK_LED, OUTPUT);
  pinMode(LOCK_BTN, INPUT_PULLUP);
  pinMode(UNLOCK_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LOCK_BTN), lockIsr, FALLING);
  attachInterrupt(digitalPinToInterrupt(UNLOCK_BTN), unlockIsr, FALLING);
  
  isLock = false;
  toSend = false;
  memset(buf, 0, MAX_SIZE);
  lastTime = 0;
  digitalWrite(UNLOCK_LED, HIGH);
}

void loop() {
  if (MODE == TX_MODE)
  {
    if (isLock && toSend)
    {
      buf[0] = 1;
      digitalWrite(LOCK_LED, HIGH);
      digitalWrite(UNLOCK_LED, LOW);
    } 
    else if (!isLock && toSend)
    {
      buf[0] = 0;
      digitalWrite(UNLOCK_LED, HIGH);
      digitalWrite(LOCK_LED, LOW);
    }
    if (toSend) 
    {
      node.send(buf, 1);
      toSend = false;
    }
  }
  else if (MODE == RX_MODE)
  {
    uint8_t len = MAX_SIZE;
    if (node.recv(buf, &len))
    {
      // Lock vehicle
      if (buf[0])
      {
        Serial.println(1);
      }
      // Unlock vehicle
      else
      {
        Serial.println(0);
      }
    }
  }
}

void lockIsr(void)
{
  unsigned long current = millis();
  if (current - lastTime > DEBOUNCE_TIME)
  {
    isLock = true;
    toSend = true;
  }
  lastTime = current;
}

void unlockIsr(void)
{
  unsigned long current = millis();
  if (current - lastTime > DEBOUNCE_TIME)
  {
    isLock = false;
    toSend = true;
  }
  lastTime = current;
}