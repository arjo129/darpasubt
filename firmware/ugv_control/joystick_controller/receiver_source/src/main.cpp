#include <Arduino.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>
#include <DataPackage.h>
#include <Motor.h>

// Transmitter radio driver related.
#define RF_RX_PIN 6
#define RF_TX_PIN 7
// Motors related.
#define DIR_1_PIN 2 // First channel is for left motors.
#define PWM_1_PIN 3
#define DIR_2_PIN 4 // Second channel is for right motors.
#define PWM_2_PIN 5
#define FWD_TREHOLD 501
#define BWD_TREHOLD 521
#define L_TREHOLD 521
#define R_TREHOLD 501
#define INPUT_MAX 1023
#define INPUT_MIN 0
#define PWM_MAX 240
#define PWM_MIN 60

// Transmitter radio driver related.
SoftwareSerial ss(RF_RX_PIN, RF_TX_PIN);
RH_RF95 receiver(ss);
uint8_t bufLen = DataPackage::DATA_LEN;
uint8_t buf[DataPackage::DATA_LEN];
DataPackage::Packet data;

// Motors related.
Motor motors(DIR_1_PIN, PWM_1_PIN, DIR_2_PIN, PWM_2_PIN);
bool m1HasStop = true;
bool m2HasStop = true;
int speed = 0;
int currSpeed = 0;
int leftSpeed = 0;
int rightSpeed = 0;

void setup() {
  Serial.begin(9600);

   // Motor related
  pinMode(PWM_1_PIN, OUTPUT);
  pinMode(DIR_1_PIN, OUTPUT);
  pinMode(PWM_2_PIN, OUTPUT);
  pinMode(DIR_2_PIN, OUTPUT);

  // Initialise receiver.
  while (!receiver.init())
  {
    Serial.println("Receiver failed, trying again in 2 seconds...");
    delay(2000);
  }
  receiver.setFrequency(434.0);
}

void loop() {
  if (receiver.available())
  {
    // Read incoming data into a buffer and then deserialise into DataPackage structure.
    receiver.recv(buf, &bufLen);
    data = DataPackage::deserialiseData(buf);
    // Forward - Backward control is active
    if (data.js1X < FWD_TREHOLD || data.js1X > BWD_TREHOLD)
    {
      // Forward
      if (data.js1X < FWD_TREHOLD)
      {
        motors.setM1Dir(DIR_FWD);
        motors.setM2Dir(DIR_FWD);

        // Get the forward PWM speed first.
        currSpeed = map(data.js1X, INPUT_MIN, FWD_TREHOLD - 1, PWM_MAX, PWM_MIN);

        // Left is also active
        if (data.js2Y > L_TREHOLD)
        {
          int leftVal = map(data.js2Y, L_TREHOLD + 1, INPUT_MAX, PWM_MIN, PWM_MAX);
          leftSpeed = currSpeed - leftVal;
          leftSpeed = leftSpeed < 0 ? 0 : leftSpeed;
          rightSpeed = currSpeed;
          motors.setM2Pwm(rightSpeed);
          motors.setM1Pwm(leftSpeed);
          Serial.println("Forward left");
        }
        // Right is also active
        else if (data.js2Y < R_TREHOLD)
        {
          int rightVal = map(data.js2Y, INPUT_MIN, R_TREHOLD - 1, PWM_MAX, PWM_MIN);
          rightSpeed = currSpeed - rightVal;
          rightSpeed = rightSpeed < 0 ? 0 : rightSpeed;
          leftSpeed = currSpeed;
          motors.setM2Pwm(rightSpeed);
          motors.setM1Pwm(leftSpeed);
          Serial.println("Forward right");
        }
        // Only forward is active
        else
        {
          motors.setM2Pwm(currSpeed);
          motors.setM1Pwm(currSpeed);
          Serial.println("Forward");
        }
        
        m1HasStop = false;
        m2HasStop = false;
      }
      // Backward
      else if (data.js1X > BWD_TREHOLD)
      {
        motors.setM1Dir(DIR_BWD);
        motors.setM2Dir(DIR_BWD);

        // Get the backward PWM speed first.
        currSpeed = map(data.js1X, BWD_TREHOLD + 1, INPUT_MAX, PWM_MIN, PWM_MAX);

        // Left is also active
        if (data.js2Y > L_TREHOLD)
        {
          int leftVal = map(data.js2Y, L_TREHOLD + 1, INPUT_MAX, PWM_MIN, PWM_MAX);
          leftSpeed = currSpeed - leftVal;
          leftSpeed = leftSpeed < 0 ? 0 : leftSpeed;
          rightSpeed = currSpeed;
          motors.setM2Pwm(rightSpeed);
          motors.setM1Pwm(leftSpeed);
          Serial.println("Backward left");
        }
        // Right is also active
        else if (data.js2Y < R_TREHOLD)
        {
          leftSpeed = currSpeed;
          int rightVal = map(data.js2Y, INPUT_MIN, R_TREHOLD - 1, PWM_MAX, PWM_MIN);
          rightSpeed = currSpeed - rightVal;
          rightSpeed = rightSpeed < 0 ? 0 : rightSpeed;
          motors.setM2Pwm(rightSpeed);
          motors.setM1Pwm(leftSpeed);
          Serial.println("Backward right");
        }
        // Only backward is active
        else
        {
          motors.setM2Pwm(currSpeed);
          motors.setM1Pwm(currSpeed);
          Serial.println("Backward");
        }
        
        m1HasStop = false;
        m2HasStop = false;
      }      
    }

    // Forward - Backward control is inactive
    if (data.js1X >= FWD_TREHOLD && data.js1X <= BWD_TREHOLD)
    {
      // If only left is active
      if (data.js2Y > L_TREHOLD)
      {
        motors.setM1Dir(DIR_BWD);
        motors.setM2Dir(DIR_FWD);
        // The wheels are only in different direction, so the PWM can be the same for both.
        currSpeed = map(data.js2Y, L_TREHOLD + 1, INPUT_MAX, PWM_MIN, PWM_MAX);
        motors.setM2Pwm(currSpeed);
        motors.setM1Pwm(currSpeed);
        Serial.println("Rotate left");

        m1HasStop = false;
        m2HasStop = false;
      }
      // If only right is active
      else if (data.js2Y < R_TREHOLD)
      {
        motors.setM1Dir(DIR_FWD);
        motors.setM2Dir(DIR_BWD);
        // The wheels are only in different direction, so the PWM can be the same for both.
        currSpeed = map(data.js2Y, INPUT_MIN, R_TREHOLD - 1, PWM_MAX, PWM_MIN);
        motors.setM2Pwm(currSpeed);
        motors.setM1Pwm(currSpeed);
        Serial.println("Rotate right");
        
        m1HasStop = false;
        m2HasStop = false;
      }
      // No joystick is active
      else
      {
        if (!m1HasStop && !m2HasStop)
        {
          currSpeed = 0;
          motors.setM2Pwm(currSpeed);
          motors.setM1Pwm(currSpeed);
          m1HasStop = true;
          m2HasStop = true;
          Serial.println("Stop");
        }
      }
    }
  }
}