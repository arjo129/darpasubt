#include <Encoder.h>

/**
 * This class allows creation of Encoder objects that measures SPEED and DISTANCE travelled.
 * 
 * updateCount() can be called  during RISING or FALLING or BOTH of an interrupt event. In this case, ticksPerRev
 * parameter of constructor must be adjusted accordingly.
 * 
 * NOTE: Be wary of the rate of interrupt events. Based on personal experience, too high of interrupt rate can cause
 * issues in Arduino Uno.
 * 
 * NOTE: Depending on the Arduino you are using (Uno, Due, Zero, etc.), int can be stored as 2-bytes or 4-bytes values.
 * In certain cases, you may wish to replace long with int to save memory. Correspondingly, be wary with the fact that
 * int are stored as 2-bytes on some boards (eg. Uno). You may then want to consider overflow cases.
 */

/**
 * @brief Construct a new Encoder:: Encoder object.
 * @details Either one of the two pins (A and B) must be an interrupt pin such that updateCount() can be called during
 * and interrupt service routine.
 * 
 * @param pinA The pin number to read first encoder output.
 * @param pinB The pin number to read second encoder output.
 * @param deltaTime The time interval (in microseconds) to measure/calculate output shaft speed.
 * @param ticksPerRev The number of ticks that will occur within one revolution of output shaft.
 */
Encoder::Encoder(uint8_t pinA, uint8_t pinB, long deltaTime, int ticksPerRev) : pinA(pinA), pinB(pinB), deltaTime(deltaTime)
{
  degPerTick = 360.0 / ticksPerRev;
  pinMode(pinB, INPUT);

#if defined(ENCODER_USE_PIN_A_PORTD) && defined(ENCODER_USE_PIN_B_PORTD)

  // Check if specified pin is valid.
  if (pinA > 7 || pinB > 7)
  {
    Serial.println("Invalid pin number specified for pin A or pin B! Check your pins again.");
    return;
  }

  // 8 bits register representation to be used for updateCount().
  pinAReg = 1 << pinA;
  pinBReg = 1 << pinB;
  
#elif defined(ENCODER_USE_PIN_A_PORTD) && defined(ENCODER_USE_PIN_B_PORTB)

  // Check if specified pin is valid.
  if (pinA > 7 || (pinB < 8 && pinB > 13))
  {
    Serial.println("Invalid pin number specified for pin A or pin B! Check your pins again.");
    return;
  }

  pinAReg = 1 << pinA;
  // We minus by 8 since PB0 is mapped to physical pin number 8, PB1 mapped to 9, so on and so forth on Uno.
  pinBReg = 1 << (pinB - 8); 

#elif defined (ENCODER_USE_PIN_A_PORTB) && defined(ENCODER_USE_PIN_B_PORTD)
  // Check if specified pin is valid.
  if ((pinA < 8 && pinA > 13) || pinB > 7)
  {
    Serial.println("Invalid pin number specified for pin A or pin B! Check your pins again.");
    return;
  }

  pinAReg = 1 << (pinA - 8);
  pinBReg = 1 << pinB;

#else
  
  // Serial.println("Unknown GPIO port specified! Check Encoder_config.h for configuration instructions.");

#endif

pinAReg = 1 << 5;
pinBReg = 1 << 2;
}

/**
 * @brief Updates ticksCount when an encoder interrupt event occurs.
 * @details This function must be called when there is a PIN CHANGE in the interrupt.
 * 
 */
void Encoder::updateCount(void)
{

#if defined(ENCODER_USE_PIN_A_PORTD) && defined(ENCODER_USE_PIN_B_PORTD)

  valA = (PIND & pinAReg) > 0;
  valB = (PIND & pinBReg) > 0;
  
#elif defined(ENCODER_USE_PIN_A_PORTD) && defined(ENCODER_USE_PIN_B_PORTB)

  valA = (PIND & pinAReg) > 0;
  valB = (PINB & pinBReg) > 0;

#elif defined (ENCODER_USE_PIN_A_PORTB) && defined(ENCODER_USE_PIN_B_PORTD)

  valA = (PINB & pinAReg) > 0;
  valB = (PIND & pinBReg) > 0;
  
#endif

  valA = (PINA & pinAReg) > 0;
  valB = (PINE & pinBReg) > 0;

  if (valA == valB)
  {
    ticksCount++;
  }
  else
  {
    ticksCount--;
  }
}

/**
 * @brief Calculates and returns the speed of the motor output shaft in degrees per second (degPerSec).
 * @details This function should be called every deltaTime for correct speed.
 * 
 * @return long The speed in degrees per second.
 */
long Encoder::getSpeed(void)
{
  oldTicksCount = newTicksCount;
  newTicksCount = ticksCount;

  // Calculate the ticks passed since last call of this function.
  int countDiff = newTicksCount - oldTicksCount;

  // Add to total count of ticks.
  totalTicksCount += countDiff;

  long degPerSec;
  // Check if ticksCount has overflowed.
  // Calculate new speed if no overflow.
  if (countDiff < 100000 && countDiff > -100000)
  {
    double intervals = 1000000.0 / deltaTime;
    double ticksPerSec = (double)countDiff * intervals;
    degPerSec = ticksPerSec * degPerTick;
    prevSpeed = degPerSec;
  }
  // Use the previous speed if overflow occurs.
  else
  {
    degPerSec = prevSpeed;
  }
  
  return degPerSec;
}

/**
 * @brief Calculates and returns the distance traveled in degrees per second (degPerSec) since the last call of this
 * function.
 * @details This function must be called regularly such that totalTicksCount would not overflow.
 * 
 * @return int The distance in degrees per second.
 */
int Encoder::getDistance(void)
{
  int distance = totalTicksCount * degPerTick;

  totalTicksCount = 0;

  return distance;
}
