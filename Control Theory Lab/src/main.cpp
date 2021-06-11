#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "Arduino.h"

#define MOTOR_A PA_1
#define MOTOR_B PA_2
#define SENSOR PB1
#define P PB0
#define D PA7
#define I PA6

#define MOTORFREQ 100
void adjust_motor(int g, int error, int lasterr);

volatile int reflectance;
volatile int set = 100;

void setup()
{
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(P, INPUT);
  pinMode(D, INPUT);
  pinMode(I, INPUT);

  // setting up the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();
}

volatile int p;
volatile int d;
volatile int i;
volatile int lasterr = 0;
volatile int g;
volatile int kp;
volatile int kd;
volatile int ki;

void loop()
{
  display.clearDisplay();
  display.setCursor(0,0);

  reflectance = analogRead(SENSOR);
  kp = analogRead(P)/100;
  kd = analogRead(D)/100;
  ki = analogRead(I)/100;

  display.println(kp);
  display.println(kd);

  int error = set - reflectance;
  p = kp*error;
  d = kd*(error - lasterr);
  // i = ki*error + i;
  // if (i > 4000) {
  //   i = 4000;
  // }
  // else if (i < -4000) {
  //   i = -4000;
  // }
  g = p + d;
  if (g > 4095) {
    g = 4095;
  } else if (g < -4095) {
    g = -4095;
  }
  adjust_motor(g, error, lasterr);
  lasterr = error;

  display.println("error: ");
  display.println(error);
  display.println("reflectance: ");
  display.println(reflectance);
  display.println("g: ");
  display.println(g);
  display.display();
}

void adjust_motor(int g, int error, int lasterr)
{
  // if error > 0, this means closer than the set value
  if (error > 0)
  {
    pwm_start(MOTOR_A, MOTORFREQ, g, RESOLUTION_12B_COMPARE_FORMAT);
    pwm_start(MOTOR_B, MOTORFREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
  }
  // if error < 0, this means further away than the set value
  if (error < 0)
  {
    pwm_start(MOTOR_A, MOTORFREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    pwm_start(MOTOR_B, MOTORFREQ, g, RESOLUTION_12B_COMPARE_FORMAT);    
  }
}
