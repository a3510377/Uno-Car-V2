#pragma once

#include <Arduino.h>

#include "UnoCarV2_LED.h"
#include "UnoCarV2_MCP23008.h"
#include "UnoCarV2_Utils.h"

class UnoCarV2 {
 public:
  UnoCarV2();
  void begin();
};
