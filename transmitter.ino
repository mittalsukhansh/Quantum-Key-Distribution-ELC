int ledPin = 13;
int bitTime = 500;
String PREAMBLE = "11110001";

// Number of bits used to encode the payload length.
// MUST match LENGTH_BITS on the receiver.
// 8 bits -> payload can be 0-255 bits long.
const int LENGTH_BITS = 8;

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

// Convert an integer length into a fixed-width binary string, MSB first.
// e.g. toBinaryString(5, 8) -> "00000101"
String toBinaryString(int value, int width) {
  String out = "";
  for (int i = width - 1; i >= 0; i--) {
    out += ((value >> i) & 0x01) ? '1' : '0';
  }
  return out;
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    // Strip anything that isn't a literal '0' or '1' so length matches
    // exactly what we're about to transmit.
    String cleanData = "";
    for (int i = 0; i < data.length(); i++) {
      if (data[i] == '0' || data[i] == '1') cleanData += data[i];
    }

    int dataLen = cleanData.length();
    if (dataLen > (1 << LENGTH_BITS) - 1) {
      Serial.println("ERROR: data too long for LENGTH_BITS, truncating");
      dataLen = (1 << LENGTH_BITS) - 1;
      cleanData = cleanData.substring(0, dataLen);
    }

    String lengthField = toBinaryString(dataLen, LENGTH_BITS);
    String frame = PREAMBLE + lengthField + cleanData;

    Serial.print("Sending: ");
    Serial.println(frame);
    Serial.print("  (preamble=");
    Serial.print(PREAMBLE);
    Serial.print(", length=");
    Serial.print(lengthField);
    Serial.print(" -> ");
    Serial.print(dataLen);
    Serial.println(" bits, then payload)");

    for (int i = 0; i < frame.length(); i++) {
      sendNRZBit(frame[i]);
    }
    digitalWrite(ledPin, LOW);
  }
}
