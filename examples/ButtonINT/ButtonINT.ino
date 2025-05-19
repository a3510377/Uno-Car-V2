#include <Arduino.h>
#include <UnoCarV2.h>

MCP23008 mcp(MCP23008_ADDRESS);
volatile bool interruptFlag = false;

void handleInterrupt();

void setup() {
  Serial.begin(115200);

  mcp.begin(true);  // true: set all pins as PULLUP

  mcp.setMode(0x03, INPUT_PULLUP);  // set pin 0 and 1 as input_pullup

  mcp.attachInterrupt(0, CHANGE);  // attach interrupt to pin 0
  mcp.attachInterrupt(1, CHANGE);  // attach interrupt to pin 1

  // set INTR pin as input
  pinMode(MCP23008_INTERRUPT_PIN, INPUT);

  // watch for interrupt
  attachInterrupt(MCP23008_INTERRUPT_NUMBER, handleInterrupt, CHANGE);
}

void loop() {
  if (interruptFlag) {
    Serial.print("Interrupt detected on pin: ");
    Serial.println(mcp.getLastInterruptPin());
    Serial.print("Pin states at time of interrupt: 0b");
    Serial.println(mcp.getCapturedInterrupt(), 2);

    interruptFlag = false;
  }
}

void handleInterrupt() {
  interruptFlag = true;
}
