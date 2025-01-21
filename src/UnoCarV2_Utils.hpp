#include "UnoCarV2_Utils.h"

template <typename T>
ValueWatch<T>::ValueWatch(void (*callback)(T, void*), void* data)
    : _value(T()), _onChange(callback), _userData(data) {}

template <typename T>
ValueWatch<T>::ValueWatch(T defaultValue, void (*callback)(T, void*),
                          void* data)
    : _value(defaultValue), _onChange(callback), _userData(data) {}

template <typename T>
ValueWatch<T>::operator T() const {
  return _value;
}

template <typename T>
void ValueWatch<T>::operator=(T newValue) {
  setValue(newValue);
}

template <typename T>
void ValueWatch<T>::setValue(T newValue) {
  if (newValue != _value) {
    _value = newValue;
    if (_onChange) {
      _onChange(_value, _userData);
    }
  }
}

template <typename T>
T ValueWatch<T>::getValue() const {
  return _value;
}
