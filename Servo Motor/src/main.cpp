#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "Arduino.h"

#define SERVO PA_0
#define SENSOR PB1
#define POT PB0

void move_arm(bool UP, int start);

// #define MOTORFREQ 100

void setup()
{
  pinMode(SERVO, OUTPUT);
  pinMode(SENSOR, INPUT);
  // pinMode(POT, INPUT);

  // setting up the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();
}

void loop()
{
  display.clearDisplay();
  display.setCursor(0,0);
  // pot = analogRead(POT);

  move_arm(true, 1500);
  delay(2000);
  move_arm(false, 500);
  delay(3000);
  // delay(1);
  // pwm_start(SERVO, 50, 500, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(1000);
  // pwm_start(SERVO, 50, 1000, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(1000);
  // pwm_start(SERVO, 50, 1500, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(1000);
  // pwm_start(SERVO, 50, 1500, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  // delay(1000);
  // delay(1000);
  display.display();
}

void move_arm(bool UP, int start) {
  int i = start;
  if (UP) {
    while (i >= 500) {
      pwm_start(SERVO, 50, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      i = i - 50;
      delay(30);
    }
  }
  else {
    while (i <= 1500) {
      pwm_start(SERVO, 50, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      i = i + 50;
      delay(30);
    }
  }
}
