#define THISFILENAME "pins.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <esp_log.h>
#include <Arduino.h>
#include <Wire.h>
#include "pins.h"

static const char *TAG = THISFILENAME;

void SetupPins() {
  ESP_LOGI(TAG, "Starting SetupPins()...");

  // Set up IO pins
  pinMode(BUTTON_BUTTON, INPUT_PULLUP); // Button input
  pinMode(SWITCH_ARM, INPUT); // Arm switch
  pinMode(VOLT_BAT, INPUT); // Analog voltage

  pinMode(LED_BUTTON, OUTPUT);  // LED ring around the button
  pinMode(LED_BUILTIN, OUTPUT); // ESP32 module built-in LED
  pinMode(BUZZER, OUTPUT); // Buzzer

  // Set up I²C interface
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(); // Start I2C bus
}
