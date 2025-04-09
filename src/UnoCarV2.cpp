#include "UnoCarV2.h"

UnoCarV2::UnoCarV2()
    : leds(WS_LED_LEN), pca9685(PCA9685_ADDRESS), mcp23008(MCP23008_ADDRESS) {}

void UnoCarV2::begin() {
  ledBegin();
  // pca9685Begin();
  mcp23008Begin();
}

void UnoCarV2::ledBegin() {
  leds.clear();
}

void UnoCarV2::pca9685Begin() {
  pca9685.begin();
}

void UnoCarV2::mcp23008Begin() {
  mcp23008.begin(true);

  mcp23008.setMode(0x03, INPUT_PULLUP);  // set pin 0 and 1 as INPUT_PULLUP
  mcp23008.setIOConfig(false, false, false, false);
  mcp23008.attachInterrupt(0, CHANGE);
  mcp23008.attachInterrupt(1, CHANGE);

  // attachInterrupt(MCP23008_INTERRUPT_NUMBER, UnoCarV2::_handleInterrupt,
  //                 CHANGE);
}

void UnoCarV2::pinMode(uint8_t pin, uint8_t mode) {
  if (pin < E0_PIN) pinMode(pin, mode);
  else if (pin < P0_PIN) mcp23008.pinMode(pin - E0_PIN, mode);
}

bool UnoCarV2::analogWrite(uint8_t pin, uint8_t value) {
  if (pin < E0_PIN) {
    ::analogWrite(pin, value);
    return true;
  } else if (pin < P0_PIN) {
    return mcp23008.digitalWrite(pin - E0_PIN, value > 128);
  } else if (pin < PM_PIN) {
    return pca9685.analogWrite(pin - P0_PIN, value);
  }
  return false;
}

uint8_t UnoCarV2::digitalRead(uint8_t pin) {
  if (pin < E0_PIN) ::digitalRead(pin);
  else if (pin < P0_PIN) mcp23008.digitalRead(pin);

  return 0;
}

void UnoCarV2::digitalWrite(uint8_t pin, uint8_t value) {
  if (pin < E0_PIN) ::digitalWrite(pin, value);
  else if (pin < P0_PIN) mcp23008.digitalWrite(pin, value);
  else if (pin < PM_PIN) pca9685.analogWrite(pin - P0_PIN, value);
}

uint8_t UnoCarV2::getMode(uint8_t pin) {
  if (pin < E0_PIN) return getMode(pin);
  else if (pin < P0_PIN) return mcp23008.getMode(pin);
  else if (pin < PM_PIN) return OUTPUT;

  return 0;
}

UnoCarV2 unoCarV2;
