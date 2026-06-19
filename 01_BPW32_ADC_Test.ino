#include<Arduino.h>
#define RX_PIN 34

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== BPW32 ADC TEST ===");
  Serial.println("Point laser ON and OFF and observe readings.");
}

void loop() {
  int adcValue = analogRead(RX_PIN);

  Serial.println(adcValue);

  delay(20);
}