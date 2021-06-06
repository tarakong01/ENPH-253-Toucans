#include <Arduino.h>
#define MOTOR_A PA_1
#define MOTOR_B PA_2

#define MOTOR_FREQ 100

void setup() {
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
}

void loop() {
  pwm_start(MOTOR_B, MOTOR_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

  for(int i = 0; i < 20; i++){
    pwm_start(MOTOR_A, MOTOR_FREQ, (i + 1) * 100, RESOLUTION_12B_COMPARE_FORMAT);
    delay(500);
  }


  pwm_stop(MOTOR_A);
  pwm_stop(MOTOR_B);

  delay(100);

  pwm_start(MOTOR_A, MOTOR_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
  
  for(int i = 0; i < 20; i++){
    pwm_start(MOTOR_B, MOTOR_FREQ, (i + 1) * 100, RESOLUTION_12B_COMPARE_FORMAT);
    delay(500);
  }

  pwm_stop(MOTOR_A);
  pwm_stop(MOTOR_B);

  delay(100);
}