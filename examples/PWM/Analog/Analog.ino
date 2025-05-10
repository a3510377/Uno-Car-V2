#include <Arduino.h>
#include <UnoCarV2.h>

void setup() {
  Serial.begin(115200);

  unoCarV2.begin();
}

void loop() {
  unoCarV2.analogWrite(P12, 0);
  unoCarV2.analogWrite(P13, 90);
  unoCarV2.analogWrite(P14, 128);
  unoCarV2.analogWrite(P15, 255);
}
