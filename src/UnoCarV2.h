#pragma once

#include <Arduino.h>

#include "UnoCarV2_LED.h"
#include "UnoCarV2_MCP23008.h"
#include "UnoCarV2_PCA9685.h"
#include "UnoCarV2_Utils.h"
#include "UnoCarV2_def.h"

#define E0_PIN (NUM_DIGITAL_PINS + 1)
#define P0_PIN (E0_PIN + 8)
#define PM_PIN (P0_PIN + 8)

#define digitalPinIsArduino(P) ((P) < E0_PIN)
#define digitalPinIsMCP23008(P) ((P) >= E0_PIN && (P) < P0_PIN)
#define digitalPinIsPCA9685(P) ((P) >= P0_PIN)
#define digitalPinToModule(P) \
  ((P) < E0_PIN ? ArduinoPin : (P) < P0_PIN ? MCP23008Pin : PCA9685Pin)

enum UnoCarV2Modules { ArduinoPin, MCP23008Pin, PCA9685Pin, UnknownPin };

enum UnoCarV2Pins {
  // Arduino pins
  D0 = 0,
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8,
  D9,
  D10,
  D11,
  D12,
  D13,

// Mega pins
#if defined(__AVR_ATmega2560__)
  D14,
  D15,
  D16,
  D17,
  D18,
  D19,
  D20,
  D21,
  D22,
  D23,
  D24,
  D25,
  D26,
  D27,
  D28,
  D29,
  D30,
  D31,
  D32,
  D33,
  D34,
  D35,
  D36,
  D37,
  D38,
  D39,
  D40,
  D41,
  D42,
  D43,
  D44,
  D45,
  D46,
  D47,
  D48,
  D49,
  D50,
  D51,
  D52,
  D53,
#endif

  // Arduino analog pins skipped (arduino is defined)
  // ...

  // MCP23008 pins
  E0 = E0_PIN,
  E1,
  E2,
  E3,
  E4,
  E5,
  E6,
  E7,

  // PCA9685 pins
  P0 = P0_PIN,
  P1,
  P2,
  P3,
  P4,
  P5,
  P6,
  P7,
  P8,
  P9,
  P10,
  P11,
  P12,
  P13,
  P14,
  P15
};

class UnoCarV2 {
 public:
  UnoCarV2();
  void begin();

  void ledBegin();
  void pca9685Begin();
  void mcp23008Begin();

  void pinMode(uint8_t pin, uint8_t mode);
  bool analogWrite(uint8_t pin, uint8_t value);
  uint8_t digitalRead(uint8_t pin);
  void digitalWrite(uint8_t pin, uint8_t value);
  uint8_t getMode(uint8_t pin);

  LEDs<WS_LED_LEN> leds;
  PCA9685 pca9685;
  MCP23008 mcp23008;

 private:
  static void _handleInterrupt() {
    _interruptFlag = true;
  }
  static bool _interruptFlag;
};

extern UnoCarV2 unoCarV2;
