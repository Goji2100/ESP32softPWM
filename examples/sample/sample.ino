#include <ESP32softPWM.hpp>

ESP32softPWM PWM;
#define MAXVAL ESP32softPWM_MAXVAL

uint8_t PWMpins[] = { 2, 4, 5, 16, 17, 18, 19, 21 };
//uint8_t PWMpins[] = { 26, 25, 17, 16, 27, 14, 12, 13,  5, 23, 19, 18, 22, 21 };

void setup() {
  Serial.begin(115200);
  for (int i = 0; (i < sizeof(PWMpins)); i++) {
    PWM.attach(i, PWMpins[i]);
  }
}

void loop() {
  PWM.update();
  for (int i = 0; (i < sizeof(PWMpins)); i++) {
    if (PWM.getcnt(i) == 0)
      PWM.set(i, random(MAXVAL), random(100, 1000));
  }
  delay(1);
}
