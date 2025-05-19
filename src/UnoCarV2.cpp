#include "UnoCarV2.h"

UnoCarV2::UnoCarV2()
    : UnoCarV2_Motor(PCA9685_ADDRESS),
      leds(WS_LED_PIN),
      mcp23008(MCP23008_ADDRESS) {}

void UnoCarV2::begin() {
  ledBegin();
  pca9685Begin();
  mcp23008Begin();
}

void UnoCarV2::noTone() {
  ::noTone(BUZZER_PIN);
}

void UnoCarV2::tone(unsigned int frequency, unsigned long duration) {
  ::tone(BUZZER_PIN, frequency, duration);
}

void UnoCarV2::ledBegin() {
  leds.clear();
}

void UnoCarV2::pca9685Begin() {
  pca9685.begin();

  pca9685.reset();
  pca9685.setOscillatorFrequency(27000000);
  pca9685.setPWMFreq(1600);
}

void UnoCarV2::mcp23008Begin() {
  mcp23008.begin(true);

  mcp23008.setMode(0x03, INPUT_PULLUP);  // set pin 0 and 1 as INPUT_PULLUP
  mcp23008.setIOConfig(false, false, false, LOW);  // set all pins as input
  mcp23008.attachInterrupt(0, CHANGE);
  mcp23008.attachInterrupt(1, CHANGE);

  /* setup interrupt */
  // set INTR pin as input
  pinMode(MCP23008_INTERRUPT_PIN, INPUT);
  // watch for interrupt
  attachInterrupt(MCP23008_INTERRUPT_NUMBER, UnoCarV2::handleInterrupt, CHANGE);
}

bool UnoCarV2::pcaAnalogWrite(uint8_t channel, float percent) {
  if (channel > 15) return false;

  percent = constrain(percent, 0.0, 100.0);
  uint16_t pwmVal = (uint16_t)(percent * 40.95);  // 4095 / 100 = 40.95

  return !!pcaAnalogWrite(channel, pwmVal);
}

bool UnoCarV2::pcaAnalogWrite(uint8_t channel, uint8_t value) {
  if (channel > 15) return false;

  uint16_t pwmVal = map(value, 0, 255, 0, 4095);

  return !!pcaAnalogWrite(channel, pwmVal);
}

bool UnoCarV2::pcaAnalogWrite(uint8_t channel, uint16_t pwmVal) {
  if (channel > 15) return false;

  pwmVal = constrain(pwmVal, 0, 4095);

  pca9685.setPin(channel, pwmVal);
  return true;
}

void UnoCarV2::pinMode(uint8_t pin, uint8_t mode) {
  if (pin < E0_PIN) ::pinMode(pin, mode);
  else if (pin < P0_PIN) mcp23008.pinMode(pin - E0_PIN, mode);
}

bool UnoCarV2::analogWrite(uint8_t pin, uint8_t value) {
  if (pin < E0_PIN) {
    ::analogWrite(pin, value);
    return true;
  } else if (pin < P0_PIN) {
    return mcp23008.digitalWrite(pin - E0_PIN, value > 128);
  } else if (pin < PM_PIN) {
    return pcaAnalogWrite(pin - P0_PIN, value);
  }
  return false;
}

uint8_t UnoCarV2::digitalRead(uint8_t pin) {
  if (pin < E0_PIN) ::digitalRead(pin);
  else if (pin < P0_PIN) mcp23008.digitalRead(pin);

  return 0;
}

void UnoCarV2::digitalWrite(uint8_t pin, uint8_t value) {
  if (pin < E0_PIN) ::digitalWrite(pin, value);
  else if (pin < P0_PIN) mcp23008.digitalWrite(pin, value);
  else if (pin < PM_PIN) {
    pcaAnalogWrite(pin - P0_PIN, (uint8_t)(value ? 255 : 0));
  }
}

uint8_t UnoCarV2::getMode(uint8_t pin) {
  if (pin < E0_PIN) return getMode(pin);
  else if (pin < P0_PIN) return mcp23008.getMode(pin);
  else if (pin < PM_PIN) return OUTPUT;

  return 0;
}

volatile bool UnoCarV2::interruptFlag = false;

void UnoCarV2::handleInterrupt() {
  interruptFlag = true;
}

UnoCarV2 unoCarV2;
