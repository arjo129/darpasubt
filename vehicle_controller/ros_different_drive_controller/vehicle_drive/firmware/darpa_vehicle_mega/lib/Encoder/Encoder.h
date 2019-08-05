#include <Arduino.h>
#include <Encoder_config.h>

#define DEF_PIN_A 2
#define DEF_PIN_B 3
#define DEF_DELTA_T 50000
#define DEF_TICKS_PER_REV 24

class Encoder
{
  public:
    Encoder(
      uint8_t pinA = DEF_PIN_A, 
      uint8_t pinB = DEF_PIN_B, 
      long deltaTime = DEF_DELTA_T, 
      int ticksPerRev = DEF_TICKS_PER_REV
      );
    void updateCount(void);
    long getSpeed(void);
    int getDistance(void);

  private:
    uint8_t pinA; // Pin number for first output
    uint8_t pinB; // Pin number for second output

    uint8_t pinAReg; // Register bit position of pin A.
    uint8_t pinBReg; // Register bit position of pin B.

    bool valA; // Used for updating of ticks count.
    bool valB; // Used for updating of ticks count.

    volatile long ticksCount = 0; // Keep track of ticks passed.
    volatile long oldTicksCount = 0;
    volatile long newTicksCount = 0;
    long totalTicksCount = 0; // Running count of ticks for distance tracking.
    long deltaTime = 0; // Time interval for calling crucial functions.

    double degPerTick = 0; // To compute speed.
    double prevSpeed = 0;
};
