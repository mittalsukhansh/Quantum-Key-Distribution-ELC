#include <AccelStepper.h>

// Motor 1
AccelStepper stepper1(
  AccelStepper::HALF4WIRE,
  4, 6, 5, 7
);

// Motor 2
AccelStepper stepper2(
  AccelStepper::HALF4WIRE,
  8, 10, 9, 11
);

// BB84 positions
const long POS_0   = 0;
const long POS_45  = 256;
const long POS_90  = 512;
const long POS_135 = 768;

void moveMotor1(long target)
{
  stepper1.moveTo(target);

  while(stepper1.distanceToGo() != 0)
  {
    stepper1.run();
  }
}

void moveMotor2(long target)
{
  stepper2.moveTo(target);

  while(stepper2.distanceToGo() != 0)
  {
    stepper2.run();
  }
}

void setup()
{
  stepper1.setMaxSpeed(800);
  stepper1.setAcceleration(400);

  stepper2.setMaxSpeed(800);
  stepper2.setAcceleration(400);
}

void loop()
{
  // Example sequence

  moveMotor1(POS_0);
  delay(1000);

  moveMotor1(POS_45);
  delay(1000);

  moveMotor1(POS_90);
  delay(1000);

  moveMotor1(POS_135);
  delay(1000);

  moveMotor2(POS_0);
  delay(1000);

  moveMotor2(POS_45);
  delay(1000);

  moveMotor2(POS_90);
  delay(1000);

  moveMotor2(POS_135);
  delay(1000);
}
