#include <Arduino.h>

#define SUCCESS 0
#define FAIL 1

#define DIR_FWD 0
#define DIR_BWD 1

// Default pin values
#define DEF_DIR_1 7
#define DEF_PWM_1 6
#define DEF_DIR_2 8
#define DEF_PWM_2 9

class Motor
{
  public:
    Motor(int pinDir1 = DEF_DIR_1, int pinPwm1 = DEF_PWM_1, int pinDir2 = DEF_DIR_2, int pinPwm2 = DEF_PWM_2);
    int setM1Dir(int dir);
    int setM2Dir(int dir);
    int getM1Dir(void);
    int getM2Dir(void);
    int setM1Pwm(int pwm);
    int setM2Pwm(int pwm);
    int getM1Pwm(void);
    int getM2Pwm(void);
    int startM1(void);
    int startM2(void);
    int stopM1(void);
    int stopM2(void);


  private:
    int pinDir1; // Direction pin for first motor channel
    int pinPwm1; // PWM pin for first motor channel
    int pinDir2; // Direction pin for second motor channel
    int pinPwm2; // PWM pin for second motor channel
    
    int dir1; // Direction for first motor channel
    int pwm1; // PWM value for first motor channel
    int dir2; // Direction for second motor channel
    int pwm2; // PWM value for first motor channel

    bool isStopped1 = true; // Flag to check if first motor is stopped.
    bool isStopped2 = true; // Flag to check if second motor is stopped.
};