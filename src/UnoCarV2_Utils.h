#pragma once

template <typename T>
class ValueWatch {
 public:
  ValueWatch(void (*callback)(T, void *), void *userData);
  ValueWatch(T defaultValue, void (*callback)(T, void *), void *userData);

  // Conversion operator to get the current value
  operator T() const;

  // Assignment operator to set new value
  void operator=(T newValue);

  // Set the new value and invoke callback if value changes
  void setValue(T newValue);

  // Get the current value
  T getValue() const;

 private:
  T _value;                      // Holds the current value
  void (*_onChange)(T, void *);  // Callback to invoke when value changes
  void *_userData;               // Data to pass to the callback
};

#include "UnoCarV2_Utils.hpp"
