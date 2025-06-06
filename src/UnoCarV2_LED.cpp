#include "UnoCarV2_LED.h"

const RGB RGB::Black(0, 0, 0);
const RGB RGB::White(255, 255, 255);
const RGB RGB::Red(255, 0, 0);
const RGB RGB::Green(0, 255, 0);
const RGB RGB::Blue(0, 0, 255);
const RGB RGB::Yellow(255, 255, 0);
const RGB RGB::Cyan(0, 255, 255);
const RGB RGB::Magenta(255, 0, 255);
const RGB RGB::Orange(255, 165, 0);
const RGB RGB::Purple(128, 0, 128);
const RGB RGB::Pink(255, 192, 203);
const RGB RGB::Brown(139, 69, 19);
const RGB RGB::Gray(128, 128, 128);
const RGB RGB::LightGray(211, 211, 211);
const RGB RGB::DarkGray(169, 169, 169);
const RGB RGB::Lime(0, 255, 0);
const RGB RGB::Teal(0, 128, 128);
const RGB RGB::Indigo(75, 0, 130);
const RGB RGB::Gold(255, 215, 0);
const RGB RGB::Silver(192, 192, 192);
const RGB RGB::Coral(255, 127, 80);
const RGB RGB::Tan(210, 180, 140);
const RGB RGB::Violet(238, 130, 238);
const RGB RGB::Salmon(250, 128, 114);
const RGB RGB::Olive(128, 128, 0);
const RGB RGB::Navy(0, 0, 128);

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

// Set the RGB color values
RGB &RGB::setRGB(uint8_t r, uint8_t g, uint8_t b) {
  _r = r;
  _g = g;
  _b = b;

  _update();

  return *this;
}

// Update the color display, triggering the callback if set
void RGB::_update() {
  if (_onChange) {
    _onChange(*this, _userData);
  }
}

// Set the callback function to be triggered when the color changes
void RGB::_setOnChange(void (*onChange)(RGB &, void *), void *userData) {
  _onChange = onChange;
  _userData = userData;
}
