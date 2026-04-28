/*
  FSR Music Box companion firmware for Arduino Mega

  Connect four FSR voltage divider outputs to A0, A1, A2, and A3.
  The webpage in main.html opens Web Serial at 115200 baud and accepts
  compact pressure lines in this format:

    0:612

  The first number is the pad index, 0-3. The second number is the raw
  Arduino ADC value, 0-1023.
*/

const int DEADZONE = 30;
const int fsrPins[4] = {A0, A1, A2, A3};

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int val = analogRead(fsrPins[i]);
    if (val > DEADZONE) {
      Serial.print(i);
      Serial.print(':');
      Serial.println(val);
    }
  }

  delay(10);
}
