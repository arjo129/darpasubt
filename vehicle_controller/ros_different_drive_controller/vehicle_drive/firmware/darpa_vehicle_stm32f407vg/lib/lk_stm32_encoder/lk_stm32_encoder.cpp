#include "lk_stm32_encoder.h"

/**
 * This class allows creation of Encoder objects that measures SPEED and DISTANCE travelled.
 * 
 * updateCount() can be called  during RISING or FALLING or BOTH of an interrupt event. In this case, ticksPerRev
 * parameter of constructor must be adjusted accordingly.
 * 
 * NOTE: Be wary of the rate of interrupt events. Depending on the microcontroller's clock speed, encoders that
 * interrupts at frequency that are too high can cause issues during runtime.
 * 
 */

/**
 * @brief Construct a new Encoder:: Encoder object.
 * @details Either one of the two pins (A and B) must be an interrupt pin such that updateCount() can be called during
 * and interrupt service routine.
 * 
 * @param pinA The pin number to read first encoder output.
 * @param pinB The pin number to read second encoder output.
 * @param pinAPort Pointer to GPIO Port type definition that corresponds to pinA.
 * @param pinBPort Pointer to GPIO Port type definition that corresponds to pinB.
 * @param deltaTime The time interval (in microseconds) to measure/calculate output shaft speed.
 * @param ticksPerRev The number of ticks that will occur within one revolution of output shaft.
 */
Encoder::Encoder(uint16_t pinA, uint16_t pinB, GPIO_TypeDef* pinAPort, GPIO_TypeDef* pinBPort, int deltaTime, int ticksPerRev) : pinA(pinA), pinB(pinB), pinAPort(pinAPort), pinBPort(pinBPort), deltaTime(deltaTime)
{
  degPerTick = 360.0 / ticksPerRev;
  TM_EXTI_Attach(pinAPort, pinA, TM_EXTI_Trigger_Rising_Falling);
}

/**
 * @brief Updates ticksCount when an encoder interrupt event occurs.
 * @details This function must be called when there is a PIN CHANGE in the interrupt.
 */
void Encoder::updateCount(void)
{
  valA = TM_GPIO_GetInputPinValue(pinAPort, pinA);
  valB = TM_GPIO_GetInputPinValue(pinBPort, pinB);

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
 * @return int The speed in degrees per second.
 */
int Encoder::getSpeed(void)
{
  oldTicksCount = newTicksCount;
  newTicksCount = ticksCount;

  // Calculate the ticks passed since last call of this function.
  int countDiff = newTicksCount - oldTicksCount;

  // Add to total count of ticks.
  totalTicksCount += countDiff;

  int degPerSec;
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

/**
 * @brief Gets the number of ticksCount count accumulated at the moment. For debugging purpose.
 * 
 * @return int The number of counts.
 */
int Encoder::getCount(void)
{
  return ticksCount;
}