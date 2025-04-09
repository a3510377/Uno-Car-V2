#pragma once

#define WS_LED_PIN A3
#define WS_LED_LEN 2

#define BUZZER_PIN 3

#define MCP23008_ADDRESS 0x20
#define MCP23008_INTERRUPT_PIN 2
#define MCP23008_INTERRUPT_NUMBER digitalPinToInterrupt(MCP23008_INTERRUPT_PIN)

#define PCA9685_ADDRESS 0x60

// IIC address: 0x60, 0x70
