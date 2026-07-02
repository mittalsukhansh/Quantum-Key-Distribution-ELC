#include <Stepper.h>

const int STEPS_PER_REV = 2048;

// Motor 1 (Alice)
Stepper alice(STEPS_PER_REV, 4, 6, 5, 7);

// Motor 2 (Bob)
Stepper bob(STEPS_PER_REV, 8, 10, 9, 11);

long alicePos = 0;
long bobPos = 0;

// Convert BB84 angle to motor steps
long angleToSteps(int angle)
{
  switch(angle)
  {
    case 0:   return 0;
    case 45:  return 256;
    case 90:  return 512;
    case 135: return 768;
    default:  return 0;
  }
}

void moveAliceTo(int angle)
{
  long target = angleToSteps(angle);

  alice.step(target - alicePos);

  alicePos = target;
}

void moveBobTo(int angle)
{
  long target = angleToSteps(angle);

  bob.step(target - bobPos);

  bobPos = target;
}

void setup()
{
  Serial.begin(9600);

  alice.setSpeed(5);
  bob.setSpeed(5);

  randomSeed(analogRead(A0));

  Serial.println("BB84 Started");
}

void loop()
{
  // Alice chooses one of 4 polarization states
  int aliceStates[4] = {0, 45, 90, 135};

  // Bob chooses one of 2 bases
  int bobBases[2] = {0, 45};

  int aliceAngle = aliceStates[random(4)];
  int bobAngle = bobBases[random(2)];

  moveAliceTo(aliceAngle);
  moveBobTo(bobAngle);

  Serial.print("Alice State = ");
  Serial.print(aliceAngle);
  Serial.print(" deg");

  Serial.print(" | Bob Basis = ");
  Serial.print(bobAngle);
  Serial.println(" deg");

  delay(3000);
}
