#include "UnoCarV2_Motor.h"

UnoCarV2_Motor::UnoCarV2_Motor(uint8_t address) : pca9685(address) {}

/**
 * @brief Set the PWM value for a PCA9685 channel
 *
 * @param channel The PCA9685 channel to set. 0-15.
 * @param pwm The PWM value to set. 0-4095.
 *
 * @return true if the PWM value was set successfully.
 */
bool UnoCarV2_Motor::_setPWM(int channel, uint16_t pwm) {
  pwm = constrain(pwm, 0, 4095);
  pca9685.setPWM(channel, 0, pwm);
  return true;
}

/**
 * @brief Drive a motor
 *
 * @param motorIndex The index of the motor to drive. 1-6.
 * @param speed The speed of the motor. -4095 to 4095.
 *
 * @return true if the motor was driven successfully.
 */
bool UnoCarV2_Motor::drive(int motorIndex, int32_t speed) {
  if (motorIndex < 1 || motorIndex > 6) return false;

  speed = constrain(speed, -4095, 4095);
  uint16_t pwm = abs(speed);
  uint8_t base = (motorIndex - 1) * 2;

  _setPWM(base, speed < 0 ? pwm : 0);
  _setPWM(base + 1, speed < 0 ? 0 : pwm);

  return true;
}

/**
 * @brief Stop all motors
 *
 * Stops all 6 motors.
 */
void UnoCarV2_Motor::stopAll() {
  for (int i = 1; i <= 6; i++) {
    stop(i);
  }
}
