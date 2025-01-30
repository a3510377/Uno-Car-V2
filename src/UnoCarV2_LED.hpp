#include "UnoCarV2_LED.h"

template <uint8_t size>
LEDs<size>::LEDs(uint8_t pin, bool immediate, float brightness)
    : _immediate(immediate), _brightness(brightness), _pin(pin) {
  _bitMask = digitalPinToBitMask(_pin);
  _bitNotMask = ~_bitMask;

  _port = digitalPinToPort(pin);
  _reg = portModeRegister(_port);
  _out = portOutputRegister(_port);

  uint8_t oldSREG = SREG;
  cli();
  // set pin as output
  *_reg |= _bitMask;
  // set pin low
  *_out &= _bitNotMask;
  SREG = oldSREG;

  for (int i = 0; i < size; i++) {
    _rgb[i] = RGB::Black;
    _rgb[i]._setOnChange(
        [](RGB &rgb, void *userData) {
          LEDs *instance = static_cast<LEDs *>(userData);
          if (instance->_immediate) {
            instance->show();
          }
        },
        this);
  }
}

template <uint8_t size>
LEDs<size>::~LEDs() {
  uint8_t oldSREG = SREG;
  cli();
  // set pin as input
  *_reg &= _bitNotMask;
  SREG = oldSREG;
}

// Display the current colors
template <uint8_t size>
void LEDs<size>::show() const {
  uint8_t oldSREG = SREG;
  cli();
  for (int i = 0; i < size; i++) {
    RGB rgb = _rgb[i];
    uint8_t r = static_cast<uint8_t>(rgb._r * _brightness),
            g = static_cast<uint8_t>(rgb._g * _brightness),
            b = static_cast<uint8_t>(rgb._b * _brightness);

    _sendPixel(r, g, b);
  }
  SREG = oldSREG;

  // >= 280 us
  delayMicroseconds(280);
}

// Clear the LED display
template <uint8_t size>
void LEDs<size>::clear() noexcept {
  setAll(RGB::Black);
}
  uint8_t oldSREG = SREG;
  cli();

  bool old = _immediate;
  _immediate = false;
  for (int i = 0; i < size; i++) {
    _rgb[i] = RGB(0, 0, 0);
  }
  _immediate = old;
  show();

  SREG = oldSREG;
}

void LEDs<size>::setAll(uint8_t r, uint8_t g, uint8_t b) noexcept {
  setAll(RGB(r, g, b));
}

// Set whether to display colors immediately
template <uint8_t size>
void LEDs<size>::setImmediate(bool immediate) {
  _immediate = immediate;
}

// Set brightness (0-255 range)
template <uint8_t size>
void LEDs<size>::setBrightness(uint8_t brightness) noexcept {
  _brightness = constrain(brightness, 0, 255) / 255.0f;

  if (_immediate) {
    show();
  }
}

// Set brightness (0.0 - 1.0 range)
template <uint8_t size>
void LEDs<size>::setBrightness(float brightness) noexcept {
  _brightness = constrain(brightness, 0.0f, 1.0f);

  if (_immediate) {
    show();
  }
}
