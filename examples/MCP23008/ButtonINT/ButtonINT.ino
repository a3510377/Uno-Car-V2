#include <Arduino.h>
#include <UnoCarV2.h>

volatile bool interruptFlag = false;

void handleInterrupt();

void setup() {
  Serial.begin(115200);

  unoCarV2.begin();

  unoCarV2.pinMode(E0, INPUT_PULLUP);
  unoCarV2.pinMode(E1, INPUT_PULLUP);

  unoCarV2.mcp23008.attachInterrupt(0, CHANGE);  // attach interrupt to pin 0
  unoCarV2.mcp23008.attachInterrupt(1, CHANGE);  // attach interrupt to pin 1

  // set INTR pin as input
  pinMode(MCP23008_INTERRUPT_PIN, INPUT);

  // watch for interrupt
  attachInterrupt(MCP23008_INTERRUPT_NUMBER, handleInterrupt, CHANGE);
}

void loop() {
  if (interruptFlag) {
    Serial.print("Interrupt detected on pin: ");
    Serial.println(unoCarV2.mcp23008.getLastInterruptPin());
    Serial.print("Pin states at time of interrupt: 0b");
    Serial.println(unoCarV2.mcp23008.getCapturedInterrupt(), 2);

    interruptFlag = false;
  }
}

void handleInterrupt() {
  interruptFlag = true;
}
