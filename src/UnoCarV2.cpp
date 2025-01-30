#include "UnoCarV2.h"

UnoCarV2::UnoCarV2() : leds(WS_LED_LEN), mcp23008(MCP23008_ADDRESS) {}

void UnoCarV2::begin() {
  Serial.begin(115200);

  ledBegin();
  mcp23008Begin();
}

void UnoCarV2::ledBegin() {
  leds.clear();
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
  switch (digitalPinToModule(pin)) {
    case ArduinoPin: {
      pinMode(pin, mode);
    } break;
    case MCP23008Pin: {
      mcp23008.pinMode(pin, mode);
    } break;
    case PCA9685Pin: {
      // TODO implement
    } break;
    default:
      return;
  }
}

bool UnoCarV2::analogWrite(uint8_t pin, uint8_t value) {
  switch (digitalPinToModule(pin)) {
    case ArduinoPin: {
      analogWrite(pin, value);
    } break;
    case PCA9685Pin: {
      // TODO implement
    } break;
    default:
      return false;
  }
  return true;
}

uint8_t UnoCarV2::digitalRead(uint8_t pin) {
  switch (digitalPinToModule(pin)) {
    case ArduinoPin: {
      return ::digitalRead(pin);
    } break;
    case MCP23008Pin: {
      return mcp23008.digitalRead(pin);
    } break;
    case PCA9685Pin: {
      // TODO implement
    } break;
    default:
      return 0;
  }
  return 0;
}

void UnoCarV2::digitalWrite(uint8_t pin, uint8_t value) {
  switch (digitalPinToModule(pin)) {
    case ArduinoPin: {
      ::digitalWrite(pin, value);
    } break;
    case MCP23008Pin: {
      mcp23008.digitalWrite(pin, value);
    } break;
    case PCA9685Pin: {
      // TODO implement
    } break;
    default:
      return;
  }
}

uint8_t UnoCarV2::getMode(uint8_t pin) {
  switch (digitalPinToModule(pin)) {
    case ArduinoPin: {
      return getMode(pin);
    } break;
    case MCP23008Pin: {
      return mcp23008.getMode(pin);
    } break;
    case PCA9685Pin: {
      // TODO implement
    } break;
    default:
      return 0;
  }
  return 0;
}

UnoCarV2 unoCarV2;
