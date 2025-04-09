#include "UnoCarV2_PCA9685.h"

bool PCA9685::begin() {
  if (!UnoCarV2_I2C::begin()) {
    return false;
  }
  reset();
  // Serial.println("test6");
  // setPWMFreq(1000);
  // Serial.println("test7");

  return true;
}

bool PCA9685::reset() {
  uint8_t mode1 = readReg(PCA9685_MODE1);
  // Serial.println("AE");
  // delay(1);
  // // P15
  // // check if already reset
  // if (mode1 & PCA9685_MODE1_RESET) {
  //   // clear sleep bit
  //   if (!writeReg(PCA9685_MODE1, mode1 & ~PCA9685_MODE1_SLEEP)) {
  //     return false;
  //   }
  //   // Allow time for oscillator to stabilize
  //   delayMicroseconds(500);
  // }
  // Serial.println("TC");
  // // reset
  // if (!writeReg(PCA9685_MODE1, mode1 | PCA9685_MODE1_RESET)) {
  //   return false;
  // }

  // // The SLEEP bit must be logic 0 for at least 500 s, before a logic 1 is
  // // written into the RESTART bit.
  // // TEST: not sure if this is needed
  // delayMicroseconds(500);
  return true;
}

bool PCA9685::analogWrite(uint8_t channel, uint8_t value) {
  if (channel > 15) return false;
  return analogWriteBig(channel, map(value, 0, 255, 0, 4095));
}

bool PCA9685::analogWriteBig(uint8_t channel, uint16_t value) {
  if (channel > 15) return false;

  if (value == 0) return setPWM(channel, 4096, 0);
  else if (value > 4096) return setPWM(channel, 0, 4096);

  return setPWM(channel, value, 4095 - value);
}

bool PCA9685::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
  if (channel > 15) return false;

  uint8_t buf[5] = {static_cast<uint8_t>(PCA9685_LED_BASE_ON_L + 4 * channel),
                    static_cast<uint8_t>(on), static_cast<uint8_t>(on >> 8),
                    static_cast<uint8_t>(off), static_cast<uint8_t>(off >> 8)};

  return write(buf, 5);
}

bool PCA9685::setPWMFreq(float freq) {
  if (freq < 24.0 || freq > 1526.0) return false;

  // Set the PWM frequency, see [7.3.5]
  float prescaleval = 25000000.0 / (4096.0 * freq) - 1.0;
  uint8_t prescale = constrain(round(prescaleval), 0x03, 0xff);

  uint8_t oldmode = readReg(PCA9685_MODE1);
  // sleep
  uint8_t newmode = (oldmode & ~PCA9685_MODE1_RESET) | PCA9685_MODE1_SLEEP;

  if (!writeReg(PCA9685_MODE1, newmode)) return false;      // go to sleep
  if (!writeReg(PCA9685_PRESCALE, prescale)) return false;  // set the prescaler
  if (!writeReg(PCA9685_MODE1, oldmode)) return false;

  delay(5);

  // This sets the MODE1 register to turn on auto increment.
  return writeReg(PCA9685_MODE1, oldmode | PCA9685_MODE1_AI);
}

bool PCA9685::sleep() {
  uint8_t mode1 = readReg(PCA9685_MODE1);

  if (_lastError != UnoCarV2_I2C_NO_ERROR) return false;
  return writeReg(PCA9685_MODE1, mode1 | PCA9685_MODE1_SLEEP);
}
