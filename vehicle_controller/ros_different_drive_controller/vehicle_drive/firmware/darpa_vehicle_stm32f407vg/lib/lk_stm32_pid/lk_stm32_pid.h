#ifndef __LK_PID_H
#define __LK_PID_H

#include "lk_stm32_motor.h"
#include "lk_stm32_encoder.h"

#define DEF_MIN_SPEED 3000 // In degrees/second.
#define DEF_GAIN 1.0

#ifdef __cplusplus

class PIDControl
{
  public:
    PIDControl(Motor *motor, Encoder *encoder);
    void correctPwm(void);
    void setGains(
      double pGain, 
      double iGain, 
      double dGain 
      );
    void setSpeed(long speed);
    void setMinSpeed(long speed);

  private:
    Motor* _motor;
    Encoder* _encoder;

    int pwm;

    long setPoint; // Desired speed in degrees/second.
    long prevSpeed; // Previous speed set.
    long minSpeed; // Minimum speed.

    double pGain;
    double iGain;
    double dGain;
    double iTerm; // Keep track of integral term.
};

#endif

#endif /* __LK_PID_H */