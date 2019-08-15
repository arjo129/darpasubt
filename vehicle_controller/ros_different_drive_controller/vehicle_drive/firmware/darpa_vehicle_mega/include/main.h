#include <Arduino.h>
#include <SpeedControl.h>
#include <Servo.h>
#include <TwistSolver.h>
#include <params.h>

// All Motors related
#define MOTOR_A_DIR_PIN 26
#define MOTOR_A_PWM_PIN 11
#define MOTOR_B_DIR_PIN 27
#define MOTOR_B_PWM_PIN 12
#define MOTOR_C_DIR_PIN 28
#define MOTOR_C_PWM_PIN 13
#define MOTOR_D_DIR_PIN 29
#define MOTOR_D_PWM_PIN 10

#define ENCODER_A_PIN_A 21 // INT0
#define ENCODER_A_PORT_A ENCODER_PIN_D_ADDR
#define ENCODER_A_PIN_B 22
#define ENCODER_A_PORT_B ENCODER_PIN_A_ADDR
#define ENCODER_B_PIN_A 20 // INT1
#define ENCODER_B_PORT_A ENCODER_PIN_D_ADDR
#define ENCODER_B_PIN_B 23
#define ENCODER_B_PORT_B ENCODER_PIN_A_ADDR
#define ENCODER_C_PIN_A 19 // INT2
#define ENCODER_C_PORT_A ENCODER_PIN_D_ADDR
#define ENCODER_C_PIN_B 24
#define ENCODER_C_PORT_B ENCODER_PIN_A_ADDR
#define ENCODER_D_PIN_A 18 // INT3
#define ENCODER_D_PORT_A ENCODER_PIN_D_ADDR
#define ENCODER_D_PIN_B 25
#define ENCODER_D_PORT_B ENCODER_PIN_A_ADDR
#define ENCODER_COMM_DELTA_T 50000
#define ENCODER_COMM_TICKS_PER_REV 24

#define SC_A_P_GAIN 0.00423
#define SC_A_I_GAIN 0.0008
#define SC_A_D_GAIN 0.0005
#define SC_B_P_GAIN 0.00423
#define SC_B_I_GAIN 0.0008
#define SC_B_D_GAIN 0.0005
#define SC_C_P_GAIN 0.00423
#define SC_C_I_GAIN 0.0008
#define SC_C_D_GAIN 0.0005
#define SC_D_P_GAIN 0.00423
#define SC_D_I_GAIN 0.0008
#define SC_D_D_GAIN 0.0005
#define SC_MIN_SPEED 0
#define SC_MAX_SPEED 60000

// All Servos related
#define SERVO_A_PWM_PIN 6
#define SERVO_B_PWM_PIN 7
#define SERVO_C_PWM_PIN 8
#define SERVO_D_PWM_PIN 46

/**
 * @brief Enumeration for checking drive parameters limits.
 * 
 */
typedef enum
{
  PARAM_OK = 0b0,
  PARAM_SERVO_EXCEED = 0b1,
  PARAM_MOTOR_EXCEED = 0b10
} CHK_PARAM_RES_t;
