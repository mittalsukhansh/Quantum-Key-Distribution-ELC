#include <Arduino.h>
#include <string.h>

#define RX_PIN 34
#define THRESHOLD 1900

const int bitTime = 500;  // Must match TX

// --- Preamble config ---
// Change ONLY this string. preambleLen and bitWindow's size are both
// derived from it automatically, so they can never get out of sync again.
#define PREAMBLE "11110001"
const int preambleLen = sizeof(PREAMBLE) - 1;   // -1 to exclude the implicit null terminator
char bitWindow[preambleLen + 1];                // +1 for our own null terminator

// Number of bits used to encode the payload length.
// MUST match LENGTH_BITS on the transmitter.
const int LENGTH_BITS = 8;

// --- Receiver state machine ---
enum RxState { SCANNING, READING_LENGTH, READING_DATA };
RxState state = SCANNING;

int lengthBitsCollected = 0;
int dataLenValue = 0;          // decoded payload length, in bits

int dataBitsCollected = 0;
char dataBuf[256];             // big enough for max length (LENGTH_BITS=8 -> up to 255 bits)

void resetToScanning() {
  state = SCANNING;
  memset(bitWindow, '0', preambleLen);
  bitWindow[preambleLen] = '\0';
  lengthBitsCollected = 0;
  dataLenValue = 0;
  dataBitsCollected = 0;
}

void setup() {
  Serial.begin(115200);
  Serial.println("RX Started");
  resetToScanning();
}

void loop() {

  int adc = analogRead(RX_PIN);
  char bit = (adc > THRESHOLD) ? '1' : '0';

  switch (state) {

    case SCANNING: {
      // Slide the window left by one and append the new bit at the end
      for (int i = 0; i < preambleLen - 1; i++) {
        bitWindow[i] = bitWindow[i + 1];
      }
      bitWindow[preambleLen - 1] = bit;

      Serial.print("Scanning: ");
      Serial.println(bitWindow);

      if (strcmp(bitWindow, PREAMBLE) == 0) {
        Serial.println(">>> PREAMBLE DETECTED, SYNCED <<<");
        state = READING_LENGTH;
        lengthBitsCollected = 0;
        dataLenValue = 0;
      }
      break;
    }

    case READING_LENGTH: {
      dataLenValue = (dataLenValue << 1) | (bit - '0');
      lengthBitsCollected++;

      if (lengthBitsCollected >= LENGTH_BITS) {
        Serial.print(">>> LENGTH = ");
        Serial.print(dataLenValue);
        Serial.println(" bits");

        if (dataLenValue == 0) {
          // Zero-length payload: nothing to read, frame's already done.
          Serial.println(">>> FRAME COMPLETE (empty payload) <<<");
          resetToScanning();
        } else if (dataLenValue > (int)sizeof(dataBuf) - 1) {
          // Sanity guard against a corrupted/garbage length field.
          Serial.println("ERROR: length exceeds buffer, resetting");
          resetToScanning();
        } else {
          state = READING_DATA;
          dataBitsCollected = 0;
        }
      }
      break;
    }

    case READING_DATA: {
      dataBuf[dataBitsCollected] = bit;
      dataBitsCollected++;
      Serial.println(bit);

      if (dataBitsCollected >= dataLenValue) {
        dataBuf[dataBitsCollected] = '\0';
        Serial.print(">>> FRAME COMPLETE, payload = ");
        Serial.println(dataBuf);
        resetToScanning();
      }
      break;
    }
  }

  delay(bitTime);
}
