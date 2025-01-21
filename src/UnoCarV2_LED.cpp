#include "UnoCarV2_LED.h"

const RGB RGB::Black = RGB(0, 0, 0);
const RGB RGB::White = RGB(255, 255, 255);
const RGB RGB::Red = RGB(255, 0, 0);
const RGB RGB::Green = RGB(0, 255, 0);
const RGB RGB::Blue = RGB(0, 0, 255);
const RGB RGB::Yellow = RGB(255, 255, 0);
const RGB RGB::Cyan = RGB(0, 255, 255);
const RGB RGB::Magenta = RGB(255, 0, 255);
const RGB RGB::Orange = RGB(255, 165, 0);
const RGB RGB::Purple = RGB(128, 0, 128);
const RGB RGB::Pink = RGB(255, 192, 203);
const RGB RGB::Brown = RGB(139, 69, 19);
const RGB RGB::Gray = RGB(128, 128, 128);
const RGB RGB::LightGray = RGB(211, 211, 211);
const RGB RGB::DarkGray = RGB(169, 169, 169);
const RGB RGB::Lime = RGB(0, 255, 0);
const RGB RGB::Teal = RGB(0, 128, 128);
const RGB RGB::Indigo = RGB(75, 0, 130);
const RGB RGB::Gold = RGB(255, 215, 0);
const RGB RGB::Silver = RGB(192, 192, 192);
const RGB RGB::Coral = RGB(255, 127, 80);
const RGB RGB::Tan = RGB(210, 180, 140);
const RGB RGB::Violet = RGB(238, 130, 238);
const RGB RGB::Salmon = RGB(250, 128, 114);
const RGB RGB::Olive = RGB(128, 128, 0);
const RGB RGB::Navy = RGB(0, 0, 128);

RGB &RGB::operator=(const RGB &other) {
  _r = other._r;
  _g = other._g;
  _b = other._b;

  _update();
  return *this;
}

RGB &RGB::operator/=(uint8_t d) {
  _r /= d;
  _g /= d;
  _b /= d;

  _update();
  return *this;
}

RGB &RGB::operator>>=(uint8_t d) {
  _r >>= d;
  _g >>= d;
  _b >>= d;

  _update();
  return *this;
}

RGB &RGB::operator&=(const RGB &other) {
  _r = (_r > other._r) ? other._r : _r;
  _g = (_g > other._g) ? other._g : _g;
  _b = (_b > other._b) ? other._b : _b;

  _update();
  return *this;
}

RGB &RGB::operator&=(uint8_t d) {
  _r = (_r > d) ? d : _r;
  _g = (_g > d) ? d : _g;
  _b = (_b > d) ? d : _b;

  _update();
  return *this;
}

RGB &RGB::operator|=(const RGB &other) {
  _r = (_r < other._r) ? other._r : _r;
  _g = (_g < other._g) ? other._g : _g;
  _b = (_b < other._b) ? other._b : _b;

  _update();
  return *this;
}

RGB &RGB::operator|=(uint8_t d) {
  _r = (_r < d) ? d : _r;
  _g = (_g < d) ? d : _g;
  _b = (_b < d) ? d : _b;

  _update();
  return *this;
}

ValueWatch<uint8_t> RGB::operator[](uint8_t i) {
  static uint8_t invalid = 0;
  uint8_t *ptr = i > 2 ? &invalid : &(&_r)[i];

  struct CallbackData {
    uint8_t *value;
    RGB *instance;
  };

  static CallbackData data_stack;
  data_stack.value = ptr;
  data_stack.instance = this;

  return ValueWatch<uint8_t>(
      *ptr,
      [](uint8_t v, void *userData) {
        CallbackData *data = static_cast<CallbackData *>(userData);

        *data->value = v;
        data->instance->_update();
      },
      &data_stack);
}

RGB &RGB::setRGB(uint8_t r, uint8_t g, uint8_t b) {
  _r = r;
  _g = g;
  _b = b;

  _update();

  return *this;
}

void RGB::_update() {
  if (_onChange) {
    _onChange(*this, _userData);
  }
}

void RGB::_setOnChange(void (*onChange)(RGB &, void *), void *userData) {
  _onChange = onChange;
  _userData = userData;
}

// --------------------------

LEDs::LEDs(uint8_t pin, bool immediate, float brightness)
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

  for (int i = 0; i < 2; i++) {
    _rgb[i] = RGB(0, 0, 0);
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

LEDs::~LEDs() {
  uint8_t oldSREG = SREG;
  cli();
  // set pin as input
  *_reg &= _bitNotMask;
  SREG = oldSREG;
}

void LEDs::show() const {
  uint8_t oldSREG = SREG;
  cli();
  for (int i = 0; i < 2; i++) {
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

void LEDs::clear() {
  uint8_t oldSREG = SREG;
  cli();

  bool old = _immediate;
  _immediate = false;
  for (int i = 0; i < 2; i++) {
    _rgb[i] = RGB(0, 0, 0);
  }
  _immediate = old;
  show();

  SREG = oldSREG;
}

void LEDs::setImmediate(bool immediate) {
  _immediate = immediate;
}

void LEDs::setBrightness(uint8_t brightness) {
  _brightness = constrain(brightness, 0, 255) / 255.0f;

  if (_immediate) {
    show();
  }
}

void LEDs::setBrightness(float brightness) {
  _brightness = constrain(brightness, 0.0f, 1.0f);

  if (_immediate) {
    show();
  }
}
