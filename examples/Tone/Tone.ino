#include <Arduino.h>
#include <UnoCarV2.h>

void setup() {
  unoCarV2.begin();

  for (int i = 0; i < 2; i++) {
    unoCarV2.tone(NOTE_C5);
    delay(120);
    unoCarV2.noTone();
    delay(120);
  }

  delay(1000);
  unoCarV2.tone(NOTE_A5, 500);
}

void loop() {}
