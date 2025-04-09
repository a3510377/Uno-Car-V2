#include "UnoCarV2_I2C.h"

#include "Arduino.h"

bool UnoCarV2_I2C::begin(bool checkConnection) {
  _resetError();

  _wire->begin();
  _begun = true;

  if (checkConnection) {
    return connected();
  }

  return true;
}

void UnoCarV2_I2C::end() {
  _resetError();

  _wire->end();
  _begun = false;
}

bool UnoCarV2_I2C::connected() {
  _resetError();

  if (!_begun && !begin(false)) {
    return false;
  }

  _wire->beginTransmission(_address);

  if (_wire->endTransmission() != 0) {
    _lastError = UnoCarV2_I2C_ERROR_CONNECTION;
    return false;
  }

  return true;
}

bool UnoCarV2_I2C::write(const uint8_t *data, size_t length, bool stop) {
  _resetError();
  _wire->beginTransmission(_address);
  if (_write(data, length) != length) {
    _lastError = UnoCarV2_I2C_ERROR_WRITE_FAILED;
    return false;
  }

  if (_wire->endTransmission(stop) != 0) {
    _lastError = UnoCarV2_I2C_ERROR_WRITE_FAILED;
    return false;
  }

  return true;
}

bool UnoCarV2_I2C::read(uint8_t *data, uint8_t length, bool stop) {
  _resetError();

  if (!_begun && !begin(false)) {
    return false;
  }

  if (_wire->requestFrom(_address, length, (uint8_t)stop) != length) {
    _lastError = UnoCarV2_I2C_ERROR_READ_FAILED;
    return false;
  }

  for (size_t i = 0; i < length; i++) {
    data[i] = _read();
  }

  return true;
}

uint8_t UnoCarV2_I2C::readReg(uint8_t reg) {
  uint8_t data[1] = {reg};
  writeThenRead(data, 1, data, 1);
  return data[0];
}

bool UnoCarV2_I2C::writeReg(uint8_t reg, uint8_t data) {
  uint8_t buf[2] = {reg, data};
  return write(buf, 2);
}

bool UnoCarV2_I2C::writeThenRead(const uint8_t *write_data, size_t write_length,
                                 uint8_t *read_data, size_t read_length,
                                 bool stop) {
  return write(write_data, write_length, stop) && read(read_data, read_length);
}
