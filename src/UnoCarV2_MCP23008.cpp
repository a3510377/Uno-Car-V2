#include "UnoCarV2_MCP23008.h"

bool MCP23008::begin(bool pullup) {
  if (!connected()) return false;

  // set all pins as input
  _writeReg(MCP23x08_REG_IODIR, 0xff);
  // set all pins to low
  _writeReg(MCP23x08_REG_GPIO, 0x00);

  if (pullup) {
    // enable pullups
    return _writeReg(MCP23x08_REG_GPPU, 0xff);
  } else {
    // disable pullups
    _writeReg(MCP23x08_REG_GPPU, 0x00);
  }

  return true;
}

bool MCP23008::connected() {
  _wire->beginTransmission(_address);

  return !_wire->endTransmission();
}

bool MCP23008::pinMode(uint8_t mode) {
  return setMode(0xff, mode);
}

bool MCP23008::pinMode(uint8_t pin, uint8_t mode) {
  if (pin > 7) return false;

  return setMode(1 << pin, mode);
}

bool MCP23008::setMode(uint8_t pinMask, uint8_t mode) {
  uint8_t reg = _readReg(MCP23x08_REG_IODIR);
  uint8_t pullUp = _readReg(MCP23x08_REG_GPPU);

  if (mode == INPUT) {
    reg |= pinMask;
    pullUp &= ~pinMask;
  } else if (mode == INPUT_PULLUP) {
    reg |= pinMask;
    pullUp |= pinMask;
  }
  // default: OUTPUT
  else {
    reg &= ~pinMask;
    pullUp &= ~pinMask;
  }

  return _writeReg(MCP23x08_REG_GPPU, pinMask) &&
         _writeReg(MCP23x08_REG_IODIR, reg);
}

bool MCP23008::digitalWrite(uint8_t pin, uint8_t value) {
  if (pin > 7) return false;

  return writeGPIO(value, 1 << pin);
}

uint8_t MCP23008::digitalRead(uint8_t pin) {
  if (pin > 7) return 0;

  return (readGPIO() >> pin) & 1 ? HIGH : LOW;
}

bool MCP23008::setInterrupts(bool mirroring, bool openDrain, uint8_t polarity) {
}

bool MCP23008::attachInterrupt(uint8_t pin, uint8_t mode) {}

bool MCP23008::detachInterrupt() {}

bool MCP23008::detachInterrupt(uint8_t pin) {}

uint8_t MCP23008::getLastInterruptPin() {}

uint8_t MCP23008::getCapturedInterrupt() {}

uint8_t MCP23008::getMode(uint8_t pin) {
  if (pin > 7) return 0;

  uint8_t reg = _readReg(MCP23x08_REG_IODIR);
  uint8_t pullUp = _readReg(MCP23x08_REG_GPPU);

  if ((reg >> pin) & 1) {
    if ((pullUp >> pin) & 1) return INPUT_PULLUP;
    return INPUT;
  } else return OUTPUT;
}

uint8_t MCP23008::readGPIO() {
  return _readReg(MCP23x08_REG_GPIO) & 0xff;
}

bool MCP23008::writeGPIO(uint8_t value, uint8_t pinMask) {
  if (pinMask == 0 || pinMask == 0xff) {
    return _writeReg(MCP23x08_REG_GPIO, value);
  }

  uint8_t out = readGPIO();

  if (value) out |= pinMask;
  else out &= pinMask;

  return _writeReg(MCP23x08_REG_GPIO, out);
}

uint8_t MCP23008::_readReg(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  if (_wire->endTransmission() != 0) {
    return 0;
  }
  uint8_t n = _wire->requestFrom(_address, (uint8_t)1);
  if (n != 1) {
    return 0;
  }
  return _wire->read();
}

bool MCP23008::_writeReg(uint8_t reg, uint8_t value) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value);
  return !_wire->endTransmission();
}
