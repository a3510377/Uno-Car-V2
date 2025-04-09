#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "UnoCarV2_I2C.h"

#define PCA9685_MODE1 0x00     // mode register 1
#define PCA9685_PRESCALE 0xFE  // prescaler for PWM output frequency

#define PCA9685_MODE1_SLEEP 0x10   // sleep bit      [4]
#define PCA9685_MODE1_AI 0x20      // auto increment [5]
#define PCA9685_MODE1_EXTCLK 0x40  // external clock [6]
#define PCA9685_MODE1_RESET 0x80   // reset bit      [7]

#define PCA9685_LED_BASE_ON_L 0x06

class PCA9685: public UnoCarV2_I2C {
 public:
  PCA9685(uint8_t address = 0x60, TwoWire* wire = &Wire)
      : UnoCarV2_I2C(address, wire) {};

  bool begin();
  bool analogWrite(uint8_t channel, uint8_t value);
  bool analogWriteBig(uint8_t channel, uint16_t value);
  bool reset();
  bool setPWM(uint8_t channel, uint16_t on, uint16_t off);
  bool setPWMFreq(float freq);

  bool sleep();
};
