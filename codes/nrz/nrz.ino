// transmitter nrz

int ledPin = 13;
int bitTime = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  Serial.println("Enter bit string:");
}

void sendNRZBit(char bit) {
  if (bit == '1') {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(bitTime);
}

void loop() {
  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');

    Serial.print("Sending: ");
    Serial.println(data);

    for (int i = 0; i < data.length(); i++) {

      if (data[i] == '0' || data[i] == '1') {
        sendNRZBit(data[i]);
      }

    }

    digitalWrite(ledPin, LOW);
  }
}
