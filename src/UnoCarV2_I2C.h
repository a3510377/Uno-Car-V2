#pragma once

#include <Wire.h>

#define UnoCarV2_I2C_ERROR_CONNECTION 1
#define UnoCarV2_I2C_ERROR_CLOCK 2
#define UnoCarV2_I2C_ERROR_WRITE_FAILED 3
#define UnoCarV2_I2C_ERROR_READ_FAILED 4

#define UnoCarV2_I2C_NO_ERROR 0

class UnoCarV2_I2C {
 public:
  UnoCarV2_I2C(uint8_t address, TwoWire *wire = &Wire)
      : _address(address), _lastError(UnoCarV2_I2C_NO_ERROR) {
    _wire = wire;
  };

  bool begin(bool checkConnection = true);
  void end();
  bool connected();
  bool read(uint8_t *data, uint8_t length, bool stop = true);
  bool write(const uint8_t *data, size_t length, bool stop = true);
  uint8_t readReg(uint8_t reg);
  bool writeReg(uint8_t reg, uint8_t data);
  bool writeThenRead(const uint8_t *write_data, size_t write_length,
                     uint8_t *read_data, size_t read_length, bool stop = false);

  inline TwoWire *getWire() {
    return _wire;
  }

  inline uint8_t getLastError() {
    return _lastError;
  }

  inline uint8_t getAddress() {
    return _address;
  }

  inline size_t _write(uint8_t data) {
#if ARDUINO >= 100
    return _wire->write(data);
#else
    return _wire->send(data);
#endif
  }

  inline size_t _write(const uint8_t *data, size_t length) {
#if ARDUINO >= 100
    return _wire->write(data, length);
#else
    return _wire->send(reg);
#endif
  }

  inline int _read() {
#if ARDUINO >= 180
    return _wire->read();
#else
    return _wire->receive();
#endif
  }

 private:
  bool _begun;
  uint8_t _address;
  TwoWire *_wire;

  inline void _resetError() {
    _lastError = UnoCarV2_I2C_NO_ERROR;
  }

 protected:
  uint8_t _lastError;
};
