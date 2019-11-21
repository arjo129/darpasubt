#include <Arduino.h>
#include <SoftwareSerial.h>
#include <RH_RF95.h>

#define OP_MODE 1
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

static unsigned long lastTime;
static uint8_t buf[MAX_SIZE];
static bool isLock;
static bool toSend;

void lockIsr(void);
void unlockIsr(void);