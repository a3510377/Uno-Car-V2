#include "UnoCarV2_MCP23008.h"

MCP23008::MCP23008(uint8_t address, TwoWire *wire)
    : UnoCarV2_I2C(address, wire) {}

/**
 * Initializes the MCP23008 device.
 * Sets all pins as inputs, initializes GPIO states, and configures pull-up
 * resistors if needed.
 * @param pullup Enables pull-up resistors if true.
 * @return true if initialization is successful, false otherwise.
 */
bool MCP23008::begin(bool pullup) {
  UnoCarV2_I2C::begin();

  // clear interrupt
  getCapturedInterrupt();
  // setup default IO config
  setIOConfig(false, false, false, false);

  // set all pins as input
  if (!writeReg(MCP23x08_REG_IODIR, 0xff)) {
    return false;
  }
  // set all pins to low
  if (!writeReg(MCP23x08_REG_GPIO, 0x00)) {
    return false;
  }

  // Configure pull-up resistors
  if (!writeReg(MCP23x08_REG_GPPU, pullup ? 0xff : 0x00)) {
    return false;
  }

  return true;
}

/**
 * Sets all pins to a given mode.
 * @param mode INPUT, INPUT_PULLUP, or OUTPUT.
 * @return true if successful, false otherwise.
 */
bool MCP23008::pinMode(uint8_t mode) {
  return setMode(0xff, mode);
}

/**
 * Configures a single pin as INPUT, INPUT_PULLUP, or OUTPUT.
 * @param pin The GPIO pin (0-7) to configure.
 * @param mode The mode (INPUT, INPUT_PULLUP, OUTPUT).
 * @return true if successful, false otherwise.
 */
bool MCP23008::pinMode(uint8_t pin, uint8_t mode) {
  if (pin > 7) {
    return false;
  }

  return setMode(1 << pin, mode);
}

/**
 * Configures multiple pins' modes using a bitmask.
 * @param pinMask A bitmask of pins to configure.
 * @param mode The mode to set for the selected pins.
 * @return true if successful, false otherwise.
 */
bool MCP23008::setMode(uint8_t pinMask, uint8_t mode) {
  uint8_t reg = readReg(MCP23x08_REG_IODIR);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  uint8_t pullUp = readReg(MCP23x08_REG_GPPU);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

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

  return writeReg(MCP23x08_REG_IODIR, reg) &&
         writeReg(MCP23x08_REG_GPPU, pinMask);
}

/**
 * Writes a HIGH or LOW value to a specific GPIO pin.
 * @param pin The GPIO pin (0-7) to modify.
 * @param value HIGH or LOW.
 * @return true if successful, false otherwise.
 */
bool MCP23008::digitalWrite(uint8_t pin, uint8_t value) {
  if (pin > 7) {
    return false;
  }

  return writeGPIO(value, 1 << pin);
}

/**
 * Reads the current state of a specific GPIO pin.
 * @param pin The GPIO pin (0-7) to read.
 * @return HIGH or LOW.
 */
uint8_t MCP23008::digitalRead(uint8_t pin) {
  if (pin > 7) {
    return false;
  }

  return (readGPIO() >> pin) & 1 ? HIGH : LOW;
}

/**
 * Configures the MCP23008 IOCON register to adjust I/O behavior.
 *
 * @param seqop If true, disables sequential operation mode (manual address
 *              management). If false, enables auto-increment for continuous
 *              reads/writes.
 * @param disableSdaSlewRate If true, disables SDA slew rate control (for
 *                           high-speed applications). If false, enables slew
 *                           rate control to reduce EMI.
 * @param openDrain If true, sets the INT pin as open-drain (for shared
 *                  interrupt lines). If false, sets the INT pin as push-pull
 *                  output.
 * @param intPolarity If true, sets INT pin to active-high for interrupt events.
 *                    If false, sets INT pin to active-low.
 * @return true if configuration is written successfully, false otherwise.
 */
bool MCP23008::setIOConfig(bool seqop, bool disableSdaSlewRate, bool openDrain,
                           bool intPolarity) {
  uint8_t reg = readReg(MCP23x08_REG_IOCON);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  if (seqop) reg |= 0x20;  // SEQOP: Disable sequential operation
  else reg &= ~0x20;

  if (disableSdaSlewRate) reg |= 0x10;  // DISSLW: Disable SDA slew rate control
  else reg &= ~0x10;

  if (openDrain) reg |= 0x04;  // ODR: Set INT pin as open-drain
  else reg &= ~0x04;

  if (intPolarity) reg |= 0x02;  // INTPOL: Set INT pin to active-high
  else reg &= ~0x02;

  return writeReg(MCP23x08_REG_IOCON, reg);
}

/**
 * Enables interrupt on a specific pin with a defined trigger mode.
 *
 * @param pin GPIO pin number (0-7).
 * @param mode Interrupt mode: LOW, CHANGE, RISING, or FALLING.
 * @return true if successful, false if an error occurs.
 */
bool MCP23008::attachInterrupt(uint8_t pin, uint8_t mode) {
  if (pin > 7) return false;

  uint8_t mask = 1 << pin;
  uint8_t gpinten = readReg(MCP23x08_REG_GPINTEN);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  uint8_t defval = readReg(MCP23x08_REG_DEFVAL);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  uint8_t intcon = readReg(MCP23x08_REG_INTCON);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  gpinten |= mask;  // enabled interrupts

  if (mode == CHANGE) intcon &= ~mask;  // interrupt on change
  else intcon |= mask;

  if (mode == LOW) defval |= mask;  // interrupt on low
  else defval &= ~mask;

  return writeReg(MCP23x08_REG_GPINTEN, gpinten) &&
         writeReg(MCP23x08_REG_DEFVAL, defval) &&
         writeReg(MCP23x08_REG_INTCON, intcon);
}

/**
 * Disables all interrupts on the MCP23008.
 *
 * @return true if successful, false if an error occurs.
 */
bool MCP23008::detachInterrupt() {
  uint8_t gpinten = readReg(MCP23x08_REG_GPINTEN);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  return writeReg(MCP23x08_REG_GPINTEN, gpinten & ~0xff);
}

/**
 * Disables interrupt for a specific pin.
 *
 * @param pin GPIO pin number (0-7).
 * @return true if successful, false if an error occurs.
 */
bool MCP23008::detachInterrupt(uint8_t pin) {
  if (pin > 7) {
    return 0;
  }

  uint8_t gpinten = readReg(MCP23x08_REG_GPINTEN);
  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;

  uint8_t mask = 1 << pin;
  return writeReg(MCP23x08_REG_GPINTEN, gpinten & ~mask);
}

/**
 * Retrieves the pin number that triggered the last interrupt.
 *
 * @return Pin number (0-7) if an interrupt occurred, 0xFF if no interrupt is
 *         detected.
 */
uint8_t MCP23008::getLastInterruptPin() {
  uint8_t intcap = readReg(MCP23x08_REG_INTF);

  uint8_t mask = 1;
  for (uint8_t pin = 0; pin < 8; pin++) {
    if (intcap & mask) return pin;
    mask <<= 1;
  }

  // No interrupt detected
  return 0xff;
}

/**
 * Retrieves the captured GPIO state at the time of the last interrupt.
 *
 * @return The 8-bit GPIO state at the moment of interrupt.
 */
uint8_t MCP23008::getCapturedInterrupt() {
  return readReg(MCP23x08_REG_INTCAP);
}

/**
 * Gets the mode (INPUT, INPUT_PULLUP, OUTPUT) of a specified pin.
 *
 * @param pin GPIO pin number (0-7).
 * @return Mode of the pin: INPUT, INPUT_PULLUP, or OUTPUT.
 */
uint8_t MCP23008::getMode(uint8_t pin) {
  if (pin > 7) {
    return 0;
  }

  uint8_t reg = readReg(MCP23x08_REG_IODIR);
  uint8_t pullUp = readReg(MCP23x08_REG_GPPU);

  if ((reg >> pin) & 1) {
    if ((pullUp >> pin) & 1) return INPUT_PULLUP;
    return INPUT;
  }
  return OUTPUT;
}

/**
 * Reads the current GPIO values.
 * @return The GPIO register value.
 */
uint8_t MCP23008::readGPIO() {
  return readReg(MCP23x08_REG_GPIO) & 0xff;
}

/**
 * Writes values to GPIO pins based on a mask.
 * @param value The value to write (bitwise operations are applied).
 * @param pinMask The bitmask of affected pins.
 * @return true if successful, false otherwise.
 */
bool MCP23008::writeGPIO(uint8_t value, uint8_t pinMask) {
  if (pinMask == 0 || pinMask == 0xff) {
    return writeReg(MCP23x08_REG_GPIO, value);
  }

  uint8_t out = readGPIO();

  if (value) out |= pinMask;
  else out &= pinMask;

  return writeReg(MCP23x08_REG_GPIO, out);
}
