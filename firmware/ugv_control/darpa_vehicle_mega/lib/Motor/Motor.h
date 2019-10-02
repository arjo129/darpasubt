#include <Arduino.h>

// Default pin values
#define DEF_DIR_1 7
#define DEF_PWM_1 6

// Function results return type.
typedef enum
{
  MOTOR_SUCCESS,
  MOTOR_FAILURE
} MOTOR_RESULT_t;

// Motor direction type.
typedef enum
{
  MOTOR_DIR_FWD,
  MOTOR_DIR_BWD
} MOTOR_DIR_t;

class Motor
{
  public:
    Motor(uint8_t pinDir = DEF_DIR_1, uint8_t pinPwm = DEF_PWM_1);
    MOTOR_RESULT_t setDir(MOTOR_DIR_t dir);
    MOTOR_DIR_t getDir(void);
    MOTOR_RESULT_t setPwm(uint8_t pwm);
    uint8_t getPwm(void);
    MOTOR_RESULT_t start(void);
    MOTOR_RESULT_t stop(void);
    void invertDir(bool inv);

  private:
    uint8_t _pinDir; // Direction pin.
    uint8_t _pinPwm; // PWM pin.
    
    MOTOR_DIR_t _dir; // Direction for motor.
    uint8_t _pwm; // PWM value for motor.
    bool _isInverted; // Determine if direction should be inverted.

    bool _isStopped = true; // Flag to check if motor is stopped.
};