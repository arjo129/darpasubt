#include <Arduino.h>
#include <Encoder_config.h>

#define DEF_PIN_A 2
#define DEF_PIN_B 3
#define DEF_DELTA_T 50000
#define DEF_TICKS_PER_REV 24

typedef enum
{
  ENCODER_PIN_A_ADDR = 0x00, 
  ENCODER_PIN_B_ADDR = 0x03, 
  ENCODER_PIN_C_ADDR = 0x06, 
  ENCODER_PIN_D_ADDR = 0x09, 
  ENCODER_PIN_E_ADDR = 0x0C, 
  ENCODER_PIN_F_ADDR = 0x0F, 
  ENCODER_PIN_G_ADDR = 0x12 
} ENCODER_PIN_ADDR_t;

class Encoder
{
  public:
    Encoder(
      uint8_t pinA = DEF_PIN_A, 
      uint8_t pinB = DEF_PIN_B,
      ENCODER_PIN_ADDR_t pinAAddrType = ENCODER_PIN_D_ADDR, 
      ENCODER_PIN_ADDR_t pinBAddrType = ENCODER_PIN_D_ADDR,
      long deltaTime = DEF_DELTA_T, 
      int ticksPerRev = DEF_TICKS_PER_REV
      );
    void updateCount(void);
    long getSpeed(void);
    int getDistance(void);

  private:
    uint8_t checkRegShift(uint8_t pin, ENCODER_PIN_ADDR_t pinPort);

    uint8_t pinA; // Pin number for first output
    uint8_t pinB; // Pin number for second output

    uint8_t pinAReg; // Register bit position of pin A.
    uint8_t pinBReg; // Register bit position of pin B.

    uint8_t pinAPortAddr; // Pin A port register address.
    uint8_t pinBPortAddr; // Pin B port register address.

    bool valA; // Used for updating of ticks count.
    bool valB; // Used for updating of ticks count.

    volatile long ticksCount = 0; // Keep track of ticks passed.
    volatile long oldTicksCount = 0;
    volatile long newTicksCount = 0;
    volatile long countDiff = 0;
    volatile long totalTicksCount = 0; // Running count of ticks for distance tracking.
    long deltaTime = 0; // Time interval for calling crucial functions.
    double intervals = 0; // Number of times getSpeed() would have been called in 1 second.

    volatile double ticksPerSec = 0;
    volatile double degPerSec = 0;
    double degPerTick = 0; // To compute speed.

    volatile int distance = 0; // Distance travlled in degrees of rotation.
};
