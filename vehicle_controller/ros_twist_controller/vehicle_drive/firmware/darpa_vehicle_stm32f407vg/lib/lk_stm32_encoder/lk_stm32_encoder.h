#ifndef __LK_ENCODER_H
#define __LK_ENCODER_H

#include "tm_stm32_exti.h"

#define DEF_PIN_A GPIO_PIN_8
#define DEF_PIN_B GPIO_PIN_10
#define DEF_PIN_A_PORT GPIOD
#define DEF_PIN_B_PORT GPIOD
#define DEF_DELTA_T 50000
#define DEF_TICKS_PER_REV 24

#ifdef __cplusplus

class Encoder
{
  public:
    Encoder(
      uint16_t pinA = DEF_PIN_A, 
      uint16_t pinB = DEF_PIN_B, 
      GPIO_TypeDef* pinAPort = DEF_PIN_A_PORT, 
      GPIO_TypeDef* pinBPort = DEF_PIN_B_PORT, 
      int deltaTime = DEF_DELTA_T, 
      int ticksPerRev = DEF_TICKS_PER_REV
      );
    void updateCount(void);
    int getSpeed(void);
    int getDistance(void);
    int getCount(void);

  private:
    uint16_t pinA; // Pin number for first output
    uint16_t pinB; // Pin number for second output
    GPIO_TypeDef* pinAPort; // Pin PORT number for first output
    GPIO_TypeDef* pinBPort; // Pin PORT number for second output

    bool valA; // Used for updating of ticks count.
    bool valB; // Used for updating of ticks count.

    volatile int ticksCount = 0; // Keep track of ticks passed.
    volatile int oldTicksCount = 0;
    volatile int newTicksCount = 0;
    int totalTicksCount = 0; // Running count of ticks for distance tracking.
    int deltaTime = 0; // Time interval for calling crucial functions.

    double degPerTick = 0; // To compute speed.
    double prevSpeed = 0;
};

#endif

#endif /* __LK_ENCODER_H */