#include <Arduino.h>
#include <SoftwareSerial.h>
#include <RH_RF95.h>

#define MAX_SIZE 10
#define LOCK_BTN 2
#define UNLOCK_BTN 3
#define LOCK_LED 11
#define UNLOCK_LED 12
// #define TX_PIN 5
// #define RX_PIN 6
#define TX_PIN 10
#define RX_PIN 11
#define DEBOUNCE_TIME 200

enum OP_MODE{
  TX_MODE,
  RX_MODE
};

static unsigned long lastTime;
static uint8_t buf[MAX_SIZE];
static bool isLock;
static bool toSend;
static const OP_MODE MODE = TX_MODE;

void lockIsr(void);
void unlockIsr(void);