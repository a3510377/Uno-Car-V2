#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

class UnoCarV2_Motor {
 public:
  UnoCarV2_Motor(Adafruit_PWMServoDriver pca9685) : _pca9685(pca9685) {}

  bool setPWM(int channel, int32_t speed) {
    speed = constrain(speed, -4095, 4095);
    setPWM(channel, (uint16_t)speed);
    return setPWM(channel, (uint16_t)speed);
  }

  void setSpeed(int speed);

  bool drive1(int speed);
  bool stop1(int speed);

  bool drive2(int speed);
  bool stop2(int speed);

  bool drive3(int speed);
  bool stop3(int speed);

  bool drive4(int speed);
  bool stop4(int speed);

  bool drive5(int speed);
  bool stop5(int speed);

  bool drive6(int speed);
  bool stop6(int speed);

  inline void stopAll() {
    setSpeed(0);
  }

 private:
  Adafruit_PWMServoDriver _pca9685;
  bool _drive(int channel, int speed);
};
