#include "UnoCarV2_LED.h"

/**
 * Constructor for the LEDs class.
 *
 * @param pin The pin number where the data line of the LEDs is connected.
 * @param immediate If true, the LEDs will be updated immediately when the
 *                  color of any one of them is changed. If false, the LEDs
 *                  will only be updated when the show() function is called.
 * @param brightness The initial brightness of the LEDs. The value should be
 *                   between 0.0 and 1.0.
 */
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

/**
 * Destructor for the LEDs class.
 *
 * This function safely resets the pin associated with the LEDs object
 * back to input mode, ensuring that the pin is properly cleaned up
 * when the object is destroyed.
 */
template <uint8_t size>
LEDs<size>::~LEDs() {
  uint8_t oldSREG = SREG;
  cli();
  // set pin as input
  *_reg &= _bitNotMask;
  SREG = oldSREG;
}

/**
 * Sets all LEDs to black (off).
 */
template <uint8_t size>
void LEDs<size>::clear() noexcept {
  setAll(RGB::Black);
}

/**
 * Sets the color of all LEDs to the specified RGB color.
 *
 * @param rgb The desired color of all LEDs.
 */
template <uint8_t size>
void LEDs<size>::setAll(RGB rgb) noexcept {
  bool old = _immediate;
  _immediate = false;
  for (int i = 0; i < size; i++) {
    _rgb[i] = rgb;
  }
  _immediate = old;
  show();
}

/**
 * Sets the color of all LEDs to the specified RGB values.
 *
 * @param r The red component of the color (0-255).
 * @param g The green component of the color (0-255).
 * @param b The blue component of the color (0-255).
 */
template <uint8_t size>
void LEDs<size>::setAll(uint8_t r, uint8_t g, uint8_t b) noexcept {
  setAll(RGB(r, g, b));
}

/**
 * Sets whether the LED display should be updated immediately when the color of
 * any LED is changed.
 *
 * @param immediate If true, the LED display will be updated immediately when
 * any LED's color is changed. If false, the display will only be updated when
 * the `show()` function is called.
 */
template <uint8_t size>
void LEDs<size>::setImmediate(bool immediate) {
  _immediate = immediate;
}

/**
 * Sets the brightness of the LED display.
 *
 * @param brightness The brightness of the display (0-255), where 255 is the
 * brightest.
 * @note If {@link setImmediate} is true, the display will be updated
 * immediately when this function is called.
 */
template <uint8_t size>
void LEDs<size>::setBrightness(uint8_t brightness) noexcept {
  _brightness = constrain(brightness, 0, 255) / 255.0f;

  if (_immediate) {
    show();
  }
}

/**
 * Sets the brightness of the LEDs to the specified value (0-1 range).
 *
 * If setImmediate(true) was called, this will immediately update the LEDs.
 *
 * @param brightness The brightness of the LEDs (0-1 range).
 */
template <uint8_t size>
void LEDs<size>::setBrightness(float brightness) noexcept {
  _brightness = constrain(brightness, 0.0f, 1.0f);

  if (_immediate) {
    show();
  }
}
