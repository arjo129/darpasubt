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
 * @details Pin A must be an interrupt pin such that updateCount() can be called during
 * and interrupt service routine.
 * 
 * @param pinA The pin number to read first encoder output (must be an interrupt pin).
 * @param pinB The pin number to read second encoder output (any non interrupt pin).
 * @param pinAAddrType The GPIO port used to read the pin A value. Must be a value of ENCODER_PIN_ADDR_t enumeration.
 * @param pinBAddrType The GPIO port used to read the pin B value. Must be a value of ENCODER_PIN_ADDR_t enumeration.
 * @param deltaTime The time interval (in microseconds) to measure/calculate output shaft speed.
 * @param ticksPerRev The number of ticks that will occur within one revolution of output shaft.
 */
Encoder::Encoder(
  uint8_t pinA, 
  uint8_t pinB, 
  ENCODER_PIN_ADDR_t pinAAddrType, 
  ENCODER_PIN_ADDR_t pinBAddrType, 
  long deltaTime, 
  int ticksPerRev
  ) : pinA(pinA), pinB(pinB), pinAPortAddr(pinAAddrType), pinBPortAddr(pinBAddrType), deltaTime(deltaTime)
{
  intervals = 1000000.0 / deltaTime;
  degPerTick = 360.0 / ticksPerRev;
  pinMode(pinB, INPUT);
  
#ifdef ENCODER_MEGA_ATMEGA2560

  // Initialise the port register bit position for both pins.
  // This is used later to read the pin HIGH/LOW value during updateCount().
  pinAReg = 1 << checkRegShift(pinA, pinAAddrType);
  pinBReg = 1 << checkRegShift(pinB, pinBAddrType);

#endif
}

/**
 * @brief Updates ticksCount when an encoder interrupt event occurs.
 * @details This function must be called when there is a PIN CHANGE in the interrupt.
 * 
 */
void Encoder::updateCount(void)
{
  valA = (_SFR_IO8(pinAPortAddr) & pinAReg) > 0;
  valB = (_SFR_IO8(pinBPortAddr) & pinBReg) > 0;

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
  countDiff = newTicksCount - oldTicksCount;

  // Add to total count of ticks.
  totalTicksCount += countDiff;

  // Check if ticksCount has overflowed.
  // Calculate new speed if no overflow.
  if (countDiff < 100000 && countDiff > -100000)
  {
    ticksPerSec = countDiff * intervals;
    degPerSec = ticksPerSec * degPerTick;
  }
  // Use the previous speed if overflow occurs.
  else
  {
    // Do nothing since degPerSec does not need to change.
  }
  
  return degPerSec;
}

/**
 * @brief Calculates and returns the distance traveled in degrees since the last call of this function.
 * @details This function must be called regularly such that totalTicksCount would not overflow.
 * 
 * @return int The distance in degrees.
 */
int Encoder::getDistance(void)
{
  distance = totalTicksCount * degPerTick;

  totalTicksCount = 0;

  return distance;
}

/**
 * @brief Utility function to figure out the bit shifting required given the pin and it's port.
 * 
 * @param pin The physical pin number.
 * @param pinPort The GPIO port connected to that pin.
 * @return uint8_t the number of shift required.
 */
uint8_t Encoder::checkRegShift(uint8_t pin, ENCODER_PIN_ADDR_t pinPort)
{
  uint8_t shift = 0;

  switch(pinPort)
  {
    case ENCODER_PIN_A_ADDR:
      shift = pin - 22; // Physical pin of PA0 is 22, so we minus that to get register bit position index.
      break;
    case ENCODER_PIN_B_ADDR:
      if (pin >= 50 && pin <= 53)
      {
        shift = (pin - 53) * -1; // Physical pin 53 is mapped to PB0 and physical pin 50 is mapped to PB3.
      }
      else if (pin >= 10 && pin <= 13)
      {
        shift = pin - 6; // Physical pin of PB4 is 10 and phyiscal pin for PB7 is 13.
      }
      break;
    case ENCODER_PIN_C_ADDR:
      shift = (pin - 37) * -1; // Physical pin for PC0 is 37 and physical pin for PC7 is 30.
      break;
    case ENCODER_PIN_D_ADDR:
      if (pin == 38)
      {
        shift = 7; // Physical pin for PD7 is 38.
      }
      else if (pin >= 18 && pin <= 21)
      {
        shift = (pin - 21) * -1; // PD0 to PD3 is mapped to physical pin 21 to 18 correspondingly.
      }
      break;
    case ENCODER_PIN_E_ADDR:
      if (pin == 0 || pin == 1)
      {
        shift = pin;
      }
      else if (pin == 2 || pin == 3)
      {
        shift = pin + 2;
      }
      else if (pin == 5)
      {
        shift = 3;
      }
      break;
    case ENCODER_PIN_F_ADDR:
      shift = pin - A0; // Note: A0 to A7 is mapped to physical pin number 54 to 61.
      break;
    case ENCODER_PIN_G_ADDR:
      if (pin == 4)
      {
        shift = 5;
      }
      else if (pin >= 39 && pin <= 41)
      {
        shift = (pin - 41) * -1; // PG0 to PG2 is mapped to physical pin 41 to 39 correspondingly.
      }
      break;
    default:
      break;
  }

  return shift;
}