#include <Arduino.h>
#include <FastLED.h>
#include <ros.h>
#include <std_msgs/ColorRGBA.h>

#define LED_PIN    13
#define NUM_LEDS   20

void messageCb(const std_msgs::ColorRGBA & toggle_msg);

//Creating a subscriber with a name toggle_led, and its callback
ros::NodeHandle nh;
ros::Subscriber<std_msgs::ColorRGBA> sub("light", &messageCb);

CRGB leds[NUM_LEDS];

void messageCb(const std_msgs::ColorRGBA & toggle_msg) 
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(toggle_msg.g, toggle_msg.r, toggle_msg.b);
    //delay(10);
    FastLED.show();
  }
}
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  
  nh.initNode();
  nh.subscribe(sub);
}


void loop () {  
  nh.spinOnce();
  delay(1);
}
