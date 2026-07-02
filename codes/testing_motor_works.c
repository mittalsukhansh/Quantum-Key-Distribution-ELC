#include <Stepper.h>

const int STEPS = 2048;

Stepper motor1(STEPS, 4, 6, 5, 7);
Stepper motor2(STEPS, 8, 10, 9, 11);

void setup()
{
  motor1.setSpeed(5);
  motor2.setSpeed(5);
}

void loop()
{
  motor1.step(512);   // 90°
  motor2.step(512);   // 90°

  delay(3000);

  motor1.step(-512);
  motor2.step(-512);

  delay(3000);
}
