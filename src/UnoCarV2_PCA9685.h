#pragma once

#include <Wire.h>

enum PCA9685_Error { PCA9685_OK };

class PCA9685 {
 public:
  PCA9685(uint8_t address = 0x60, TwoWire* wire = &Wire)
      : _address(address), _wire(wire) {};

  bool begin();
  bool connected();
  bool analogWrite(uint8_t channel, uint16_t value);
  bool setPWM(uint8_t channel, uint16_t on, uint16_t off);
  bool setPWMFreq(uint16_t freq);
  bool setAllPWM(uint16_t on, uint16_t off);
  bool setAllPWMFreq(uint16_t freq);

 private:
  uint8_t _address;
  TwoWire* _wire;

  PCA9685_Error _lastError = PCA9685_OK;
};
