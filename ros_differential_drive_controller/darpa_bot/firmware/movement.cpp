#include <ros.h>
#include <Arduino.h>
#include "PinChangeInt.h"
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>

#define AIN1 11
#define AIN2 5
#define BIN1 6
#define BIN2 3
#define ENCODER_L 8 
#define DIRECTION_L 4
#define ENCODER_R 7
#define DIRECTION_R 2

long lticks = 0;
long rticks = 0;

double right_motor_pwm = 0.0;
double left_motor_pwm = 0.0;

ros::NodeHandle nh;

std_msgs::Int16 r_encoder_msg;
ros::Publisher r_encoder_pub("rwheel", &r_encoder_msg);
std_msgs::Int16 l_encoder_msg;
ros::Publisher l_encoder_pub("lwheel", &l_encoder_msg);

void set_rmotor(const std_msgs::Float32& msg) {
    right_motor_pwm = msg.data;
}

void set_lmotor(const std_msgs::Float32& msg) {
    left_motor_pwm = msg.data;
}

ros::Subscriber<std_msgs::Float32> rsub("rmotor_cmd", &set_rmotor);
ros::Subscriber<std_msgs::Float32> lsub("lmotor_cmd", &set_lmotor);

void set_pwm(int motora, int motorb) {
    if (motora > 0) {
        analogWrite(AIN2, motora);
        digitalWrite(AIN1, LOW);
    } else {
        digitalWrite(AIN1, HIGH);
        analogWrite(AIN2, 255 + motora); 
    }

    if (motorb > 0) {
        digitalWrite(BIN2, LOW);
        analogWrite(BIN1, motorb);
    } else {
        analogWrite(BIN1, 255 + motorb);
        digitalWrite(BIN2, HIGH); 
    }
}

void read_encoder_r() {
    if (digitalRead(ENCODER_R) == LOW) {
        if (digitalRead(DIRECTION_R) == LOW) {
            rticks++;
        } else {
            rticks--;
        }
    } else {   
        if (digitalRead(DIRECTION_R) == LOW) {
            rticks--; 
        } else {
            rticks++;
        }
    }
}

void read_encoder_l() {
    if (digitalRead(ENCODER_L) == LOW) {    
        if (digitalRead(DIRECTION_L) == LOW) {
            lticks--;  
        } else {
            lticks++;
        }
    } else {   
        if (digitalRead(DIRECTION_L) == LOW) {
            lticks++; 
        } else {
            lticks--;
        }
    }
}

void setup() {
    pinMode(AIN1, OUTPUT);         
    pinMode(AIN2, OUTPUT);          
    pinMode(BIN1, OUTPUT);          
    pinMode(BIN2, OUTPUT);
    pinMode(ENCODER_L, INPUT);   
    pinMode(DIRECTION_L, INPUT);      
    pinMode(ENCODER_R, INPUT);        
    pinMode(DIRECTION_R, INPUT);             

    attachInterrupt(0, read_encoder_r, CHANGE);
    attachPinChangeInterrupt(4, read_encoder_l, CHANGE);

    nh.initNode();
    nh.advertise(r_encoder_pub);
    nh.advertise(l_encoder_pub);
    nh.subscribe(rsub);
    nh.subscribe(lsub);
}

void loop() {
    set_pwm(left_motor_pwm, right_motor_pwm);
    r_encoder_msg.data = rticks;
    r_encoder_pub.publish(&r_encoder_msg);
    l_encoder_msg.data = lticks;
    l_encoder_pub.publish(&l_encoder_msg);
    nh.spinOnce();
}
