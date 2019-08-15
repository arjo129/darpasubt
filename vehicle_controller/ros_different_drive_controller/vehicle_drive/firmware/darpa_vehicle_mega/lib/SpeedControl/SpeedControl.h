#include <Motor.h>
#include <Encoder.h>

#define DEF_MIN_SPEED 3000 // In degrees/second.
#define DEF_GAIN 1.0

class SpeedControl
{
  public:
    SpeedControl(Motor *motor, Encoder *encoder);
    long correctPwm(void);
    void setGains(double pGain, double iGain, double dGain);
    void setSpeed(long speed);
    void setMinSpeed(long speed);

  private:
    Motor *motor;
    Encoder *encoder;

    long pwm;

    long setPoint; // Desired speed in degrees/second.
    long prevSpeed; // Previous speed set.
    long minSpeed; // Minimum speed.

    double pGain;
    double iGain;
    double dGain;
    double iTerm; // Keep track of integral term.
    double iLimit; // Integral term limit to prevent hunting.
};