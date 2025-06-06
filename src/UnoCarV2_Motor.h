#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

enum MotorChannel { M1 = 1, M2, M3, M4, M5, M6 };

class UnoCarV2_Motor {
 public:
  UnoCarV2_Motor(uint8_t address);

  void stopAll(void);

  bool drive(int motorIndex, int32_t speed);

  // clang-format off
  inline bool stop(int motorIndex) { return drive(motorIndex, 0); }

  inline bool drive1(int speed) { return drive(M1, speed); }
  inline bool drive2(int speed) { return drive(M2, speed); }
  inline bool drive3(int speed) { return drive(M3, speed); }
  inline bool drive4(int speed) { return drive(M4, speed); }
  inline bool drive5(int speed) { return drive(M5, speed); }
  inline bool drive6(int speed) { return drive(M6, speed); }

  inline bool driveA(int speed) { return drive1(speed); }
  inline bool driveB(int speed) { return drive2(speed); }
  inline bool driveC(int speed) { return drive3(speed); }
  inline bool driveD(int speed) { return drive4(speed); }
  inline bool driveE(int speed) { return drive5(speed); }
  inline bool driveF(int speed) { return drive6(speed); }

  inline bool stop1() { return stop(M1); }
  inline bool stop2() { return stop(M2); }
  inline bool stop3() { return stop(M3); }
  inline bool stop4() { return stop(M4); }
  inline bool stop5() { return stop(M5); }
  inline bool stop6() { return stop(M6); }

  inline bool stopA() { return stop1(); }
  inline bool stopB() { return stop2(); }
  inline bool stopC() { return stop3(); }
  inline bool stopD() { return stop4(); }
  inline bool stopE() { return stop5(); }
  inline bool stopF() { return stop6(); }
  // clang-format on

  Adafruit_PWMServoDriver pca9685;

 private:
  bool _setPWM(int channel, uint16_t pwm);
};
