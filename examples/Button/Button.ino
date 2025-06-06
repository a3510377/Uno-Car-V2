#include <Arduino.h>
#include <UnoCarV2.h>

void setup() {
  Serial.begin(115200);

  unoCarV2.begin();
}

void loop() {
  // if (!unoCarV2.digitalRead(E0)) {
  if (!unoCarV2.digitalRead(BTN0)) {
    Serial.println("Button 0 pressed!");
    delay(200);
  }

  // if (!unoCarV2.digitalRead(E1)) {
  if (!unoCarV2.digitalRead(BTN1)) {
    Serial.println("Button 1 pressed!");
    delay(200);
  }
}
