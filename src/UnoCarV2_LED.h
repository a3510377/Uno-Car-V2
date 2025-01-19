#pragma once

#include <Arduino.h>

#include "UnoCarV2_Utils.h"
#include "UnoCarV2_def.h"

class RGB {
 public:
  /// Default constructor: initializes the RGB color to black (0, 0, 0)
  RGB() : _r(0), _g(0), _b(0) {}

  /// Constructor that accepts RGB values directly (0 to 255)
  RGB(uint8_t r, uint8_t g, uint8_t b) noexcept : _r(r), _g(g), _b(b) {}

  /// Constructor that takes a 24-bit integer color code (0xRR_GG_BB)
  RGB(uint32_t code) noexcept
      : _r((code >> 16) & 0xFF), _g((code >> 8) & 0xFF), _b(code & 0xFF) {}

  /// Returns the RGB as a 32-bit integer in the form 0xAA_RR_GG_BB (Alpha +
  /// RGB)
  uint32_t as_uint32_t() const noexcept {
    return 0xff000000 | (static_cast<uint32_t>(_r) << 16) |
           (static_cast<uint32_t>(_g) << 8) | static_cast<uint32_t>(_b);
  }

  /// Copy constructor
  inline RGB(const RGB &other) noexcept = default;

  /// Assignment operator
  RGB &operator=(const RGB &other) noexcept;
  /// Divide each color component by a scalar value
  RGB &operator/=(uint8_t d) noexcept;
  /// Right-shift each color component by a specified amount
  RGB &operator>>=(uint8_t d) noexcept;
  /// "and" operator brings each channel down to the lower of the two values
  RGB &operator&=(const RGB &other) noexcept;
  /// @copydoc operator&=
  RGB &operator&=(uint8_t d) noexcept;
  /// "or" operator with a scalar value to apply max limits to RGB components
  RGB &operator|=(const RGB &other) noexcept;
  /// @copydoc operator|=
  RGB &operator|=(uint8_t d) noexcept;
  /// Returns the value of the specified channel
  ValueWatch<uint8_t> operator[](uint8_t i) noexcept;

  /// Set RGB values manually
  RGB &setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept;

  inline RGB &operator=(const uint32_t &other) noexcept {
    setCode(other);

    return *this;
  }

  /// Adds two RGB colors together with saturation (clamps at 255)
  inline RGB operator+(const RGB &other) const noexcept {
    return RGB(constrain(_r + other._r, 0, 255),
               constrain(_g + other._g, 0, 255),
               constrain(_b + other._b, 0, 255));
  }

  /// Subtracts one RGB color from another with clamping (min 0)
  inline RGB operator-(const RGB &other) const noexcept {
    return RGB(constrain(_r - other._r, 0, 255),
               constrain(_g - other._g, 0, 255),
               constrain(_b - other._b, 0, 255));
  }

  /// Checks if two RGB colors are equal
  inline bool operator==(const RGB &other) const noexcept {
    return _r == other._r && _g == other._g && _b == other._b;
  }

  /// Checks if two RGB colors are not equal
  inline bool operator!=(const RGB &other) const noexcept {
    return !(*this == other);
  }

  inline uint8_t operator[](uint8_t i) const noexcept {
    return i > 2 ? 0 : (&_r)[i];
  }

  inline RGB &setCode(uint32_t code) noexcept {
    return setRGB((code >> 16) & 0xff, (code >> 8) & 0xff, code & 0xFF);
  }

  /// Get the Red component (0 to 255)
  inline uint8_t getRed() const noexcept {
    return _r;
  }

  /// Get the Green component (0 to 255)
  inline uint8_t getGreen() const noexcept {
    return _g;
  }

  /// Get the Blue component (0 to 255)
  inline uint8_t getBlue() const noexcept {
    return _b;
  }

  static const RGB Black;
  static const RGB White;
  static const RGB Red;
  static const RGB Green;
  static const RGB Blue;
  static const RGB Yellow;
  static const RGB Cyan;
  static const RGB Magenta;
  static const RGB Orange;
  static const RGB Purple;
  static const RGB Pink;
  static const RGB Brown;
  static const RGB Gray;
  static const RGB LightGray;
  static const RGB DarkGray;
  static const RGB Lime;
  static const RGB Teal;
  static const RGB Indigo;
  static const RGB Gold;
  static const RGB Silver;
  static const RGB Coral;
  static const RGB Tan;
  static const RGB Violet;
  static const RGB Salmon;
  static const RGB Olive;
  static const RGB Navy;

 private:
  uint8_t _r, _g, _b;

  void *_userData = nullptr;  // Data to pass to the callback
  void (*_onChange)(RGB &, void *) = nullptr;

  /// call the onChange callback
  void _update() noexcept;

  /// Set the onChange callback
  void _setOnChange(void (*onChange)(RGB &, void *), void *userData) noexcept;

  friend class LEDs;
};

class LEDs {
 public:
  LEDs(uint8_t pin, bool immediate = true, float brightness = 0.5f);
  ~LEDs();

  /// Update the LEDs
  void show() const;

  /// Clear the LEDs
  void clear() noexcept;

  /// Set whether to immediately update the LEDs
  void setImmediate(bool immediate);

  /// Set brightness for all LEDs (scale factor from 0.0 to 1.0)
  void setBrightness(float brightness) noexcept;

  /// Set brightness for all LEDs (scale factor from 0 to 255)
  void setBrightness(uint8_t brightness) noexcept;

  inline RGB operator[](int index) const {
    return _rgb[index];
  }

  inline RGB &operator[](int index) {
    return _rgb[index];
  }

  /// Set brightness for all LEDs (scale factor from 0 to 255)
  inline void setBrightness(int brightness) noexcept {
    setBrightness(static_cast<uint8_t>(constrain(brightness, 0, 255)));
  }

 private:
  RGB _rgb[2];
  bool _immediate;
  float _brightness;

  uint8_t _pin, _port, _bitMask, _bitNotMask;
  volatile uint8_t *_reg, *_out;

  inline void _sendBit(uint8_t bit) const {
    volatile uint8_t hi = *_out | _bitMask, lo = *_out & _bitNotMask;

    // T0H -> 220ns ~ 380ns
    // T1H -> 580ns ~ 1us
    // T0L -> 580ns ~ 1us
    // T1L -> 220ns ~ 380ns

    asm volatile(
        "st   %a[port],  %[hi]  \n\t"
        "nop                    \n\t"
        "1: sbiw %[onCycles], 1 \n\t"
        "brne 1b                \n\t"
        "st   %a[port],  %[lo]  \n\t"
        "2: sbiw %[offCycles], 1\n\t"
        "brne 2b\n\t"
        //
        ::[port] "e"(_out),
        [hi] "r"(hi), [lo] "r"(lo),

        // 1:380ns
        // 2:630ns
        // 3:880ns
        // ...
        [onCycles] "w"(bit ? 3 : 1), [offCycles] "w"(bit ? 1 : 3));
  }

  inline void _sendByte(uint8_t byte) const {
    for (uint8_t bit = 0; bit < 8; bit++) {
      _sendBit(bitRead(byte, 7));

      byte <<= 1;
    }
  }

  inline void _sendPixel(uint8_t r, uint8_t g, uint8_t b) const {
    _sendByte(g);
    _sendByte(r);
    _sendByte(b);
  }
};
