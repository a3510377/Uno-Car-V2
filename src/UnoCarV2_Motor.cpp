#include "UnoCarV2_Motor.h"

bool UnoCarV2_Motor::_setPWM(int channel, uint16_t pwm) {
  pwm = constrain(pwm, 0, 4095);
  pca9685.setPWM(channel, 0, pwm);
  return true;
}

bool UnoCarV2_Motor::drive(int motorIndex, int32_t speed) {
  if (motorIndex < 1 || motorIndex > 6) return false;

  speed = constrain(speed, -4095, 4095);
  uint16_t pwm = abs(speed);
  uint8_t base = (motorIndex - 1) * 2;

  _setPWM(base, speed < 0 ? pwm : 0);
  _setPWM(base + 1, speed < 0 ? 0 : pwm);

  return true;
}

void UnoCarV2_Motor::stopAll() {
  for (int i = 1; i <= 6; i++) {
    stop(i);
  }
}
