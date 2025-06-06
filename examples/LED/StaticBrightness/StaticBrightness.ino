#include "UnoCarV2.h"

void setup() {
  unoCarV2.begin();
  unoCarV2.ledBegin();

  // Set brightness to 70%
  unoCarV2.leds.setBrightness(0.7f);

  // Set first LED to red, second LED to blue
  unoCarV2.leds[0] = RGB::Red;
  unoCarV2.leds[1] = RGB::Blue;

  // Show LEDs
  unoCarV2.leds.show();
}

void loop() {}
