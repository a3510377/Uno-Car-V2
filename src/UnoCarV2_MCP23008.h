#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "UnoCarV2_I2C.h"

// Registers                          description     default value
#define MCP23x08_REG_IODIR 0x00    // direction           [1] 1:input, 0:output
#define MCP23x08_REG_IPOL 0x01     // input polarity      [0] 1:inv    0:not inv
#define MCP23x08_REG_GPINTEN 0x02  // interrupt enable    [0] 1:enable 0:disable
#define MCP23x08_REG_DEFVAL 0x03   // interrupt def       [0] Mismatch trigger
#define MCP23x08_REG_INTCON 0x04   // interrupt control   [0] 1:DEFVAL 0:prev
#define MCP23x08_REG_IOCON 0x05    // IO control register [0] see datasheet P14
#define MCP23x08_REG_GPPU 0x06     // pull up resistors   [0] 1:enable 0:disable
#define MCP23x08_REG_INTF 0x07     // interrupt flag      [0] 1:raised 0:no
#define MCP23x08_REG_INTCAP 0x08   // interrupt capture   [x] read only
#define MCP23x08_REG_GPIO 0x09     // General Purpose IO  [0] 1:high 0:low
#define MCP23x08_REG_OLAT 0x0A     // output latch        [0] 1:high 0:low

class MCP23008: public UnoCarV2_I2C {
 public:
  MCP23008(uint8_t address = 0x20, TwoWire *wire = &Wire);

  bool begin(bool pullup = false);
  bool pinMode(uint8_t mode);
  bool pinMode(uint8_t pin, uint8_t mode);
  bool setMode(uint8_t pinMask, uint8_t mode);
  bool digitalWrite(uint8_t pin, uint8_t value);
  uint8_t digitalRead(uint8_t pin);

  bool setIOConfig(bool seqop, bool disableSdaSlewRate, bool openDrain,
                   bool intPolarity);
  bool attachInterrupt(uint8_t pin, uint8_t mode);
  bool detachInterrupt();
  bool detachInterrupt(uint8_t pin);
  uint8_t getLastInterruptPin();
  uint8_t getCapturedInterrupt();

  uint8_t getMode(uint8_t pin);

  uint8_t readGPIO();
  bool writeGPIO(uint8_t value, uint8_t pinMask);
};
