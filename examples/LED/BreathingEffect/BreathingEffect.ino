#include <Arduino.h>
#include <UnoCarV2.h>

void setup() {
  unoCarV2.begin();

  // Set both LEDs to green initially
  unoCarV2.leds[0] = RGB::Green;  // RGB(0, 255, 0);
  unoCarV2.leds[1] = RGB::Red;    // RGB(255, 0, 0);
}

void loop() {
  // Gradually increase brightness from 0 to 1
  for (float b = 0.0f; b <= 1.0f; b += 0.05f) {
    unoCarV2.leds.setBrightness(b);
    unoCarV2.leds.show();
    delay(50);
  }

  // Gradually decrease brightness from 1 to 0
  for (float b = 1.0f; b >= 0.0f; b -= 0.05f) {
    unoCarV2.leds.setBrightness(b);
    unoCarV2.leds.show();
    delay(50);
  }
}
