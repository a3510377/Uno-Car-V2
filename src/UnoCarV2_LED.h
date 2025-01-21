#pragma once

#include <Arduino.h>

#include "UnoCarV2_Utils.h"

class RGB {
 public:
  RGB() : _r(0), _g(0), _b(0) {}
  RGB(uint8_t r, uint8_t g, uint8_t b) noexcept : _r(r), _g(g), _b(b) {}
  RGB(uint32_t code) noexcept
      : _r((code >> 16) & 0xFF), _g((code >> 8) & 0xFF), _b(code & 0xFF) {}

  uint32_t as_uint32_t() const noexcept {
    return 0xff000000 | (static_cast<uint32_t>(_r) << 16) |
           (static_cast<uint32_t>(_g) << 8) | static_cast<uint32_t>(_b);
  }

  inline RGB(const RGB &other) noexcept = default;

  RGB &operator=(const RGB &other) noexcept;
  RGB &operator/=(uint8_t d) noexcept;
  RGB &operator>>=(uint8_t d) noexcept;
  RGB &operator&=(const RGB &other) noexcept;
  RGB &operator&=(uint8_t d) noexcept;
  RGB &operator|=(const RGB &other) noexcept;
  RGB &operator|=(uint8_t d) noexcept;
  ValueWatch<uint8_t> operator[](uint8_t i) noexcept;

  RGB &setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept;

  inline RGB &operator=(const uint32_t &other) noexcept {
    setCode(other);

    return *this;
  }

  inline RGB operator+(const RGB &other) const noexcept {
    return RGB(constrain(_r + other._r, 0, 255),
               constrain(_g + other._g, 0, 255),
               constrain(_b + other._b, 0, 255));
  }

  inline RGB operator-(const RGB &other) const noexcept {
    return RGB(constrain(_r - other._r, 0, 255),
               constrain(_g - other._g, 0, 255),
               constrain(_b - other._b, 0, 255));
  }

  inline bool operator==(const RGB &other) const noexcept {
    return _r == other._r && _g == other._g && _b == other._b;
  }

  inline bool operator!=(const RGB &other) const noexcept {
    return !(*this == other);
  }

  inline uint8_t operator[](uint8_t i) const noexcept {
    return i > 2 ? 0 : (&_r)[i];
  }

  inline RGB &setCode(uint32_t code) noexcept {
    return setRGB((code >> 16) & 0xff, (code >> 8) & 0xff, code & 0xFF);
  }

  inline uint8_t getRed() const noexcept {
    return _r;
  }

  inline uint8_t getGreen() const noexcept {
    return _g;
  }

  inline uint8_t getBlue() const noexcept {
    return _b;
  }

  // Define common colors as static constants
  static const RGB Black, White, Red, Green, Blue, Yellow, Cyan, Magenta;
  static const RGB Orange, Purple, Pink, Brown, Gray, LightGray, DarkGray;
  static const RGB Lime, Teal, Indigo, Gold, Silver, Coral, Tan, Violet, Salmon,
      Olive, Navy;

 private:
  uint8_t _r, _g, _b;

  void *_userData = nullptr;
  void (*_onChange)(RGB &, void *) = nullptr;

  void _update() noexcept;
  void _setOnChange(void (*onChange)(RGB &, void *), void *userData) noexcept;

  template <uint8_t size>
  friend class LEDs;
};

template <uint8_t size>
class LEDs {
 public:
  LEDs(uint8_t pin, bool immediate = true, float brightness = 0.5f);
  ~LEDs();

  void show() const;
  void clear() noexcept;
  void setImmediate(bool immediate);
  void setBrightness(float brightness) noexcept;
  void setBrightness(uint8_t brightness) noexcept;

  // Get RGB at index
  inline RGB operator[](int index) const {
    return _rgb[index];
  }

  // Set RGB at index
  inline RGB &operator[](int index) {
    return _rgb[index];
  }

  // Set brightness
  inline void setBrightness(int brightness) noexcept {
    setBrightness(static_cast<uint8_t>(constrain(brightness, 0, 255)));
  }

 private:
  RGB _rgb[size];
  bool _immediate;
  float _brightness;

  uint8_t _pin, _port, _bitMask, _bitNotMask;
  volatile uint8_t *_reg, *_out;

  // Send a single bit to the LED strip
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

  // Send a byte (8 bits) to the LED strip
  inline void _sendByte(uint8_t byte) const {
    for (uint8_t bit = 0; bit < 8; bit++) {
      _sendBit(bitRead(byte, 7));

      byte <<= 1;
    }
  }

  // Send an RGB color to the LED strip
  inline void _sendPixel(uint8_t r, uint8_t g, uint8_t b) const {
    _sendByte(g);
    _sendByte(r);
    _sendByte(b);
  }
};

#include "UnoCarV2_LED.hpp"
