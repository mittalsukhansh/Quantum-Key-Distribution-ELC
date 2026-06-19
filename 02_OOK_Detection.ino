#include<Arduino.h>
#define RX_PIN 34

#define THRESHOLD 1400

void setup() {
  Serial.begin(115200);

  Serial.println("=== OOK DETECTOR ===");
}

void loop() {

  int adc = analogRead(RX_PIN);

  if(adc > THRESHOLD)
  {
    Serial.print("1");
  }
  else
  {
    Serial.print("0");
  }

  delay(5);
}